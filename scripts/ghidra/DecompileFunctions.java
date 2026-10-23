// Emit bounded decompiler hypotheses; temporary type/fixup changes are rolled back.
//@author N0zoM1z0
//@category TH11

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.ParameterImpl;
import ghidra.program.model.data.UnsignedIntegerDataType;
import ghidra.program.model.data.VoidDataType;
import ghidra.program.model.symbol.SourceType;
import java.io.File;
import java.io.PrintWriter;

public class DecompileFunctions extends GhidraScript
{
    @Override
    protected void run() throws Exception
    {
        String[] args = getScriptArgs();
        if (args.length < 2)
            throw new IllegalArgumentException(
                "usage: DecompileFunctions.java OUTPUT [u32@|u32-fast1@|cookie@]ADDRESS...");
        int transaction = currentProgram.startTransaction("temporary decompiler ABI hypotheses");
        DecompInterface decompiler = new DecompInterface();
        try
        {
            for (int i = 1; i < args.length; ++i)
            {
                if (!args[i].contains("@"))
                    continue;
                String[] selector = args[i].split("@", 2);
                Function function = getFunctionAt(toAddr(selector[1]));
                if (function == null)
                    throw new IllegalArgumentException("no function for hypothesis: " + args[i]);
                if (selector[0].equals("cookie"))
                {
                    function.setReturnType(VoidDataType.dataType, SourceType.USER_DEFINED);
                    function.setCallFixup("security_check_cookie");
                }
                else if (selector[0].equals("u32") || selector[0].equals("u32-fast1"))
                {
                    function.setReturnType(UnsignedIntegerDataType.dataType, SourceType.USER_DEFINED);
                    if (selector[0].equals("u32-fast1"))
                    {
                        function.setCallingConvention("__fastcall");
                        function.replaceParameters(Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,
                            true, SourceType.USER_DEFINED,
                            new ParameterImpl("buttons", UnsignedIntegerDataType.dataType, currentProgram));
                    }
                }
                else
                    throw new IllegalArgumentException("unknown hypothesis: " + selector[0]);
            }
            decompiler.toggleCCode(true);
            decompiler.toggleSyntaxTree(true);
            if (!decompiler.openProgram(currentProgram))
                throw new IllegalStateException(decompiler.getLastMessage());
            try (PrintWriter output = new PrintWriter(new File(args[0]), "UTF-8"))
            {
                for (int i = 1; i < args.length; ++i)
                {
                    if (args[i].startsWith("cookie@"))
                        continue;
                    Address address = toAddr(args[i].contains("@") ? args[i].split("@", 2)[1] : args[i]);
                    Function function = getFunctionAt(address);
                    if (function == null)
                    {
                        output.printf("/* no function at %s */%n", address);
                        continue;
                    }
                    DecompileResults result = decompiler.decompileFunction(function, 300, monitor);
                    output.printf("/* %s at %s */%n", function.getName(true), address);
                    if (!result.decompileCompleted())
                        output.printf("/* decompile failed: %s */%n", result.getErrorMessage());
                    else
                        output.println(result.getDecompiledFunction().getC());
                }
            }
        }
        finally
        {
            decompiler.dispose();
            currentProgram.endTransaction(transaction, false);
        }
    }
}
