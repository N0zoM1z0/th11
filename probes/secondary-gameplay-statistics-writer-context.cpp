// Narrow, non-runnable link entry for the 00437A70 statistics-file owner.
extern int __cdecl SaveSecondaryGameplayStatisticsFile();
extern "C" int __cdecl SecondaryGameplayStatisticsWriterProbe()
{
    return SaveSecondaryGameplayStatisticsFile();
}
