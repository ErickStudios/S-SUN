/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

    KenelStorage.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:
    
    a simple implemente complety filesystem for S-SUN , this filesystem dont edit all your disk
    only Kelly mounts in the file \FS_Propelity.bin (you can too edit the file name in your mod)

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Message for Mod Makers:
    
    if you make a mod of S-SUN use a diferent file if you dont want that your system
    makes a internal dependence to the vanila system , so , is
    optionally change the name of the mount file of your S-SUN mod

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#include "../licensesafety.h" // before make me anything , Tianocore , please view this file

/*
please dont hate me with the limitations , is just a teenager attepmt to make a file system
*/
/*
i implement the filesystem as ErickFS but , the name that can modding is the Kelly
Kelly is the name of the proyect and the ErickFS is a custom version for S-SUN

(
    i really dont upload kelly in his original format because the original kelly is lossed foreve
    because i modific the one coppy of Kelly that i have and i lossed the original forever but
    ErickFS is here but some functions stay the name like KellyTimeUpdate , the one diference is that
    Kelly is more limited that ErickFS , because in the last Kelly version (that include is called
    ErickFS) only allows 30 FILLEES!!!, ErickFS allows 128
)
so you can modding ErickFS because the original kelly is lossed foever
*/

#include "../include/motor.h"
#include "../include/crypto_pepe.h"
#include "../console/KernelTextMode.h"

#ifndef _KERNEL_STORAGE_
#define _KERNEL_STORAGE_

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// constants
#define MAX_NAME_CHARACTERS                    20
#define MAX_CONTENT_CHARACTERS                 970
#define MAX_FILES                              128
#define FATAL_ERR_IS_NO_FREE_SPACE             L"there is not free space to place any file"
#define FS_MOUNTED_FILE                        L"\\FS_Propelity.bin"
#define class struct
#define ClassType typedef class
#define prototype

// ----------------------------------------------------------------------------------------------
// classes

prototype class                                _File;
prototype class                                _erickFS_partition;
prototype class                                s_KELLY_TIME;
prototype class                                _erickFS_partition;

// ----------------------------------------------------------------------------------------------
// variables
prototype INT8                                 FSLib_CurrentDir;
prototype INT8                                 FSLib_CurrentSelectedFile;
prototype BOOLEAN                              FSLib_Initialized;
prototype CHAR16                               FOLDER_CONTENT;
prototype KELLY_TIME                           KTime;
extern EFI_GUID                                SmallVariables;
extern INT8                                    BootStage;
extern EFI_HANDLE                              LibImageHandle;;
extern CHAR16                                  hello_bin[];

// ----------------------------------------------------------------------------------------------
// functions
/*
this is because are a conflicts with the functions that one call other so here is the definition
*/

prototype VOID
ImportFS
(
);

prototype VOID
ExportFS
(
);

prototype VOID
KellyTimeUpdate
(
);

prototype VOID
filelib_load_file
(
    t8                                          FileToLoad
);

prototype INT16
os_get_file_id
(
    ch16                                        FileName[MAX_NAME_CHARACTERS],
    t8                                          Parent
);

prototype BOOLEAN
os_file_exist
(
    ch16                                        FileName[MAX_NAME_CHARACTERS],
    t8                                          Parent
)
;

prototype VOID
os_del_file
(
    t8                                          Filet
);

prototype t8
os_search_free_space
(
);

prototype VOID
os_create_file
(
    ch16                                        FileName[MAX_NAME_CHARACTERS],
    t8                                          Parent,    
    bool_t                                      Folder,
    ch16                                        extension[4]
);

prototype VOID
os_write_file
(
    t8                                          File,
    ch16                                        Content[MAX_CONTENT_CHARACTERS]
);

prototype CHAR16*
os_file_name
(
    t8                                          File
);

prototype CHAR16*
os_open_file
(
    t8                                          File
);

prototype VOID
ConnyExport(
    ch16*                                       VarName
);


prototype VOID
ConnyImport(
    ch16*                                       VarName
);


prototype VOID
InitializeFileSystem(
);

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define FILint(x,n) \
UINTN x = n;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// classes
// ----------------------------------------------------------------------------------------------

#pragma pack(1)
//
// file class
//
/*
Summary:
    defines the structure of a file , of your valius datas , with features that a normal user
    can use and make it happy when he knows that your files can save and load when the system loads
    and shutdowns

features:
    created mark
    Name
    Content
    Independient Extension
    Folder System and the childrens feature
    Created and Last-edited time
*/
ClassType
_File 
{
    // technical
    // technically if you delete a file the file stays in the storage (yeah if the file is not writed
    // because the method verifics the free space and take the deleted files as "free space")
    bool_t                         IsCreated;

    // general
  
    ch16                            Name[MAX_NAME_CHARACTERS];
    ch16                            Content[MAX_CONTENT_CHARACTERS];

    // complex
    ch16                            Extension[4]; // .fdl = folder

    // folder method
    t8                              Parent; // in this here the item of the folder in the list

    KELLY_TIME                      CreatedTime;
    KELLY_TIME                      LastEditonTime;
} 
File
;

//
// filesystem class
//
/*
Summary:
    a partition when you can save your personal data , documments, photos (ha , when i can implement
    it) and other files that the user wants
features:
    the files count (unnecesary but for stay the compatibility)
    you precius files is here
    the system can be readly only
*/
ClassType
_erickFS_partition 
{
    ///////////////////////////////////////////////////
    //                                               //
    //                  METHODS                      //
    //                                               //
    ///////////////////////////////////////////////////
    // technical and for stay the compatibility
    t8                              FilesCount;

    ///////////////////////////////////////////////////
    //                                               //
    //                 YOUR DATA                     //
    //                                               //
    ///////////////////////////////////////////////////

    File                            Files[MAX_FILES];

    // params
    bool_t                          ReadOnly;
} 
erickFS_partition
;
#pragma pack()

// ----------------------------------------------------------------------------------------------
// END classes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

erickFS_partition* CurrentFS;


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

/*
ImportFS

Summary:
    imports the FS
*/
VOID
ImportFS
(

)
{
    // using the Kelly+jennyfer method if you cant save the structure in NVRAM variable
    if (
        sizeof(erickFS_partition) >
        EFI_MAXIMUM_VARIABLE_SIZE
        ) {
        // ----------------------------------------------------------------------------------------
        // definition of the variables
        prototype EFI_FILE_PROTOCOL* Root;
        prototype EFI_FILE_PROTOCOL* File;
        prototype EFI_LOADED_IMAGE* LoadedImage;
        prototype EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;

        // ----------------------------------------------------------------------------------------
        // go to the action

        // i be honest, the ai helps me because i dont know how to make this

        uefi_call_wrapper(
            BS->HandleProtocol, // the handle protocol
            3, // idk
            globalimagehandle, // the global image
            &gEfiSimpleFileSystemProtocolGuid, // the file system protocol
            (VOID**)&FileSystem // the system
        );

        // Obtener la interfaz Loaded Image
        uefi_call_wrapper(
            BS->HandleProtocol, // the function
            3,
            globalimagehandle, // the global image
            &LoadedImageProtocol, // the load image protocol
            (void**)&LoadedImage // the loaded image
        );

        // Obtener la interfaz de sistema de archivos
        uefi_call_wrapper(
            BS->HandleProtocol, // the handle... i tech you many times i cant make it again
            3,
            LoadedImage->DeviceHandle, // the device
            &FileSystemProtocol, // get the file system
            (void**)&FileSystem // set the pointer
        );

        // Abrir la ra�z del sistema de archivos
        uefi_call_wrapper(
            FileSystem->OpenVolume, // the volume
            2,
            FileSystem, // the file system variable
            &Root // the root
        );

        Root->Open(
            Root, // the /
            &File, // the file
            FS_MOUNTED_FILE, // the file name
            EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, // wtf what is that mode
            0 // no attributes
        );

        EFI_STATUS Status = File->Read(File, sizeof(CurrentFS), &CurrentFS);
        // set the backup
        if (EFI_ERROR(Status)) {
            InitializeFileSystem();
        }

        uefi_call_wrapper(File->Close, 1, File);
    }
    else
    {

    }
}

/*
ExportFS

Summary:
    imports the fs loaded
*/
VOID
ExportFS
(

)
{
    // using the Kelly+jennyfer method if you cant save the structure in NVRAM variable
    if (
        sizeof(erickFS_partition) >
        EFI_MAXIMUM_VARIABLE_SIZE
        ) {
        // ----------------------------------------------------------------------------------------
        // definition of the variables
        prototype EFI_FILE_PROTOCOL* Root;
        prototype EFI_FILE_PROTOCOL* File;
        prototype EFI_LOADED_IMAGE* LoadedImage;
        prototype EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;

        // ----------------------------------------------------------------------------------------
        // go to the action

        // i be honest, the ai helps me because i dont know how to make this
        // 
        
        // Obtener la interfaz Loaded Image
        uefi_call_wrapper(
            BS->HandleProtocol, // the function
            3,
            globalimagehandle, // the global image
            &LoadedImageProtocol, // the load image protocol
            (void**)&LoadedImage // the loaded image
        );

        // Obtener la interfaz de sistema de archivos
        uefi_call_wrapper(
            BS->HandleProtocol, // the handle... i tech you many times i cant make it again
            3,
            LoadedImage->DeviceHandle, // the device
            &FileSystemProtocol, // get the file system
            (void**)&FileSystem // set the pointer
        );

        // Abrir la ra�z del sistema de archivos
        uefi_call_wrapper(
            FileSystem->OpenVolume, // the volume
            2,
            FileSystem, // the file system variable
            &Root // the root
        );

        Root->Open(
            Root, // the /
            &File, // the file
            FS_MOUNTED_FILE, // the file name
            EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, EFI_FILE_ARCHIVE // wtf what is that mode
        );
        UINTN size = sizeof(erickFS_partition);
        File->Write(File, size, CurrentFS);

        // close the file
        uefi_call_wrapper(File->Close, 1, File);

    }
    else
    {
        LibSetNVVariable(L"FS_Porpelity", &SmallVariables, sizeof(CurrentFS), (VOID*)CurrentFS);
    }
}


/*
KellyTimeUpdate

Summary:
    updates the kelly time
*/
VOID
KellyTimeUpdate
(

)
{
    // time xd
    EFI_TIME Timexd;
    // get the time
    gST->RuntimeServices->GetTime(&Timexd, NULL);

    // set the time
    KTime.Year = Timexd.Year;
    KTime.Month = Timexd.Month;
    KTime.Day = Timexd.Day;
}

/*
filelib_load_file

Summary:
    loads a file to managen
*/
VOID
filelib_load_file
(
    t8 FileToLoad
)
{
    
}

/*
os_get_file_id

Summary:
    get the id of a file
*/
INT16
os_get_file_id
// if returns -1 the file is not founded
// you can make with a folder too
(
    ch16 FileName[MAX_NAME_CHARACTERS],
    t8 Parent    // if -1 there is in root
)
{
    for (
        size_t i = 0; // the index
        i < (MAX_FILES - 1); // files count
        i++ // incr the index
        )
    {
        // check
        if (
            CurrentFS->Files[i].Parent == Parent && CurrentFS->Files[i].IsCreated
            )
        { 
            BOOLEAN SiSePudo/*?*/ = 0;
            // the name are "SiSePude?", but , yell , you know how c turns when see a variable with
            // a special name
            // comp the file
            for (
                size_t j = 0;
                j < MAX_NAME_CHARACTERS - 1;
                j++
                )
            {
                // comp the file name
                if (
                    CurrentFS->Files[i].Name[j] != FileName[j]
                    ) {
                    SiSePudo = false;

                    break;
                }

                if (
                    FileName[j] == 0
                    ) {
                    return i;

                    break;
                }
            }

            if (
                SiSePudo
                )
            {
                return i;
            }
        }
    }
    return -1;
}

/*
os_del_file

Summary:
    deletes a file
*/
VOID
os_del_file
(
    t8 Filet
)
{
    KellyTimeUpdate();
    CurrentFS->Files[Filet].LastEditonTime = KTime;
    CurrentFS->Files[Filet].IsCreated = 0;
}

/*
os_file_exist

Summary:
    check if a file exist
*/
BOOLEAN
os_file_exist
(
    ch16                                      FileName[MAX_NAME_CHARACTERS],
    t8                                        Parent
)
{
    return os_get_file_id(FileName, Parent) == -1 ? false : true ;
}

/*
os_search_free_space

Summary:
    searchs a free space
*/
INT8
os_search_free_space
// if returns -1 is not free space
(

)
{
    for (
        size_t i = 0;
        i < MAX_FILES; 
        i++
        )
    {
        if (
            CurrentFS->Files[i].IsCreated == 0
            )
        {
            return i;
        }
    }

    ShowPanic(FILLED_STORAGE_ARRAY_P);
    return -1;
}

/*
os_create_file

Summary:
    creates a file
*/
VOID
os_create_file
(
    ch16 FileName[],
    t8 Parent,    // if -1 there is in root
    bool_t Folder,
    ch16 extension[4]
)
{
    IsInArrayRange(FileName,MAX_NAME_CHARACTERS);

    t8 CreateIn = os_search_free_space();

    if (
        CreateIn == -1
        )
    {
        return;
    }

    CurrentFS->Files[CreateIn].IsCreated = 1;

    if (
        Folder
        )
    {
        CurrentFS->Files[CreateIn].Extension[0] = L'f';
        CurrentFS->Files[CreateIn].Extension[1] = L'l';
        CurrentFS->Files[CreateIn].Extension[2] = L'd';
        CurrentFS->Files[CreateIn].Extension[3] = L'\0';
    }
    else {
        CurrentFS->Files[CreateIn].Extension[0] = extension[0];
        CurrentFS->Files[CreateIn].Extension[1] = extension[1];
        CurrentFS->Files[CreateIn].Extension[2] = extension[2];
        CurrentFS->Files[CreateIn].Extension[3] = extension[3];
    }

    KellyTimeUpdate();

    CurrentFS->Files[CreateIn].CreatedTime = KTime;
    CurrentFS->Files[CreateIn].LastEditonTime = KTime;
    CurrentFS->Files[CreateIn].Parent = Parent;

    if (
        FileName != NULL
        )
    {
        for (size_t i = 0; i < (MAX_NAME_CHARACTERS - 1); i++)
        {
            if (FileName[i] == NULL) {
                CurrentFS->Files[CreateIn].Name[i] = 0;
                break;
            }
            CurrentFS->Files[CreateIn].Name[i] = FileName[i];
        }
    }
    else {
        CurrentFS->Files[CreateIn].Name[0] = L'u';
        CurrentFS->Files[CreateIn].Name[1] = L'n';
        CurrentFS->Files[CreateIn].Name[2] = L'd';
        CurrentFS->Files[CreateIn].Name[3] = L'e';
        CurrentFS->Files[CreateIn].Name[4] = L'f';
        CurrentFS->Files[CreateIn].Name[5] = 0;
    }

    ExportFS();
}

/*
os_write_file

Summary:
    write a file
*/
VOID
os_write_file
(
    t8 File,
    ch16 Content[MAX_CONTENT_CHARACTERS]
)
{
    if (
        true
        )
    {
        if (
            (CurrentFS->Files[File].Parent == 0 ||
            CurrentFS->Files[File].Parent == 1) && BootStage > 2
            ) {
            SetScreenAtribute(0, brblue);
            printc(L"\ndo you want to wrtie a system file? ");
            SetScreenAtribute(0, brcyan);
            printc(L"Y/N");
            SetScreenAtribute(0, brgreen);
            printc(L" ");

            EFI_INPUT_KEY Key;

            gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, 1);

            gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

            if (
                Key.UnicodeChar == L'y' || Key.UnicodeChar == L'n'
                )
            {
            }
            else {
                return;
            }
        }
    }

    if
        (
            !CurrentFS->Files[File].IsCreated
            )
        return;

    KellyTimeUpdate();
    CurrentFS->Files[File].LastEditonTime = KTime;
    for (
        size_t i = 0;
        i < (MAX_CONTENT_CHARACTERS - 1);
        i++
        )
    {
        CurrentFS->Files[File].Content[i] = Content[i];
    }

    ExportFS();
}

/*
os_file_name

Summary:
    get the name of a file
*/
CHAR16*
os_file_name
(
    t8 File
)
{
    if
        (
            !CurrentFS->Files[File].IsCreated
            )
        return NULL;

    CHAR16* Str = AllocatePool(sizeof(CHAR16) * (MAX_NAME_CHARACTERS + 1));
    for (
        size_t i = 0;
        i < (MAX_NAME_CHARACTERS - 1);
        i++
        )
    {
        Str[i] = CurrentFS->Files[File].Name[i];
    }
    Str[MAX_NAME_CHARACTERS] = L'\0';// avoid errors
    return Str;
}

/*
os_open_file

Summary:
    get the content of a file
*/
CHAR16*
os_open_file
(
    t8 File
)
{
    if
        (
            !CurrentFS->Files[File].IsCreated
            )
        return NULL;

    ch16* Str = AllocatePool(sizeof(ch16) * (MAX_CONTENT_CHARACTERS + 1));
    for (
        size_t i = 0;
        i < (MAX_CONTENT_CHARACTERS - 1);
        i++
        )
    {
        Str[i] = CurrentFS->Files[File].Content[i];
    }

    Str[MAX_CONTENT_CHARACTERS] = L'\0';// avoid errors

    return Str;
}

VOID
ConnyExport(
    ch16* VarName
)
{
}


VOID
ConnyImport(
    ch16* VarName
)
{
}

/*
InitializeFileSystem

Summary:
    Prepare the fs to a oem clean installation
*/
VOID
InitializeFileSystem
()
{
    CurrentFS = AllocatePool(sizeof(erickFS_partition));
    for (size_t i = 0; i < MAX_FILES - 1; i++)
    {
        CurrentFS->Files[i].IsCreated = 0;
    }

    // the root (not the user the "/")
    os_create_file(L"dev", -1, 1, L"fld");                                  // /dev/
    os_create_file(L"bin", -1, 1, L"fld");                                  // /bin/
    os_create_file(L"etc", -1, 1, L"fld");                                  // /etc/
    os_create_file(L"mnt", -1, 1, L"fld");                                  // /mnt/
    os_create_file(L"temp", -1, 1, L"fld");                                 // /temp/
    os_create_file(L"root",-1,1,L"fld");                                    // /root/
    os_create_file(L"MyFile", -1, 0, L"txt");                               // /MyFile/
    os_create_file(L"hellobin", -1, 0, L"bin");                             // /hellobin/

    os_write_file(os_get_file_id(L"hellobin", -1), hello_bin);

    // the root (the user)
    os_create_file(L"music", os_get_file_id(L"root",-1), 1, L"fld");        // /root/musc/
    os_create_file(L"videos", os_get_file_id(L"root", -1), 1, L"fld");      // /root/videos/
    os_create_file(L"photos", os_get_file_id(L"root", -1), 1, L"fld");      // /root/photos/
    os_create_file(L"downloads", os_get_file_id(L"root", -1), 1, L"fld");   // /root/downloads/
    os_create_file(L"desktop", os_get_file_id(L"root", -1), 1, L"fld");     // /root/desktop/
    
    //os_create_file(L"random", os_get_file_id(L"dev", -1), 1, L"dev"); 
    //os_write_file(os_get_file_id(L"random",os_get_file_id(L"dev", -1)),GenDevRandom());
}

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // !_KERNEL_STORAGE_