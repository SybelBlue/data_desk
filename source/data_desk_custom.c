typedef struct DataDeskCustom
{
    DataDeskInitCallback *InitCallback;
    DataDeskFileCallback *FileCallback;
    DataDeskConstantCallback *ConstantCallback;
    DataDeskStructCallback *StructCallback;
    DataDeskEnumCallback *EnumCallback;
    DataDeskFlagsCallback *FlagsCallback;
    DataDeskDeclarationCallback *DeclarationCallback;
    DataDeskCleanUpCallback *CleanUpCallback;
    DataDeskProcedureHeaderCallback *ProcedureHeaderCallback;
    
#if BUILD_WIN32
    HANDLE custom_dll;
#elif BUILD_LINUX
    void *custom_dll;
#endif
    
}
DataDeskCustom;

static DataDeskCustom
DataDeskCustomLoad(char *custom_dll_path)
{
    DataDeskCustom custom = {0};
    
#if BUILD_WIN32
    custom.custom_dll = LoadLibraryA(custom_dll_path);
    if(custom.custom_dll)
    {
        Log("Custom layer successfully loaded from \"%s\".", custom_dll_path);
        custom.InitCallback                = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomInitCallback");
        custom.FileCallback                = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomFileCallback");
        custom.ConstantCallback            = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomConstantCallback");
        custom.StructCallback              = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomStructCallback");
        custom.EnumCallback                = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomEnumCallback");
        custom.FlagsCallback               = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomFlagsCallback");
        custom.DeclarationCallback         = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomDeclarationCallback");
        custom.CleanUpCallback             = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomCleanUpCallback");
        custom.ProcedureHeaderCallback     = (void *)GetProcAddress(custom.custom_dll, "DataDeskCustomProcedureHeaderCallback");
    }
#elif BUILD_LINUX
    custom.custom_dll = dlopen(custom_dll_path, RTLD_NOW);
    if(custom.custom_dll)
    {
        Log("Custom layer successfully loaded from \"%s\".", custom_dll_path);
        custom.InitCallback                = dlsym(custom.custom_dll, "DataDeskCustomInitCallback");
        custom.FileCallback                = dlsym(custom.custom_dll, "DataDeskCustomFileCallback");
        custom.ConstantCallback            = dlsym(custom.custom_dll, "DataDeskCustomConstantCallback");
        custom.StructCallback              = dlsym(custom.custom_dll, "DataDeskCustomStructCallback");
        custom.EnumCallback                = dlsym(custom.custom_dll, "DataDeskCustomEnumCallback");
        custom.FlagsCallback               = dlsym(custom.custom_dll, "DataDeskCustomFlagsCallback");
        custom.DeclarationCallback         = dlsym(custom.custom_dll, "DataDeskCustomDeclarationCallback");
        custom.CleanUpCallback             = dlsym(custom.custom_dll, "DataDeskCustomCleanUpCallback");
        custom.ProcedureHeaderCallback     = dlsym(custom.custom_dll, "DataDeskCustomProcedureHeaderCallback");
    }
#endif
    
    if(!custom.InitCallback && !custom.FileCallback &&
       !custom.ConstantCallback && !custom.StructCallback &&
       !custom.EnumCallback && !custom.FlagsCallback &&
       !custom.DeclarationCallback && !custom.CleanUpCallback &&
       !custom.ProcedureHeaderCallback)
    {
        fprintf(stdout, "WARNING: No callbacks successfully loaded in custom layer\n");
    }
    
    return custom;
}

static void
DataDeskCustomUnload(DataDeskCustom *custom)
{
    
#if BUILD_WIN32
    FreeLibrary(custom->custom_dll);
#elif BUILD_LINUX
    dlclose(custom->custom_dll);
#endif
    
    custom->InitCallback = 0;
    custom->FileCallback = 0;
    custom->ConstantCallback = 0;
    custom->StructCallback = 0;
    custom->EnumCallback = 0;
    custom->FlagsCallback = 0;
    custom->DeclarationCallback = 0;
    custom->CleanUpCallback = 0;
    custom->ProcedureHeaderCallback = 0;
    custom->custom_dll = 0;
}
