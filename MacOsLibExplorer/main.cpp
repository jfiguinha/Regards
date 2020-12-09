#include <wx/init.h>
#include <wx/utils.h> 
#include <wx/dir.h> 
#if wxVERSION_NUMBER < 2900
    #include <wx/string.h>
#else
    #include <wx/crt.h>
#endif
#include <wx/textfile.h>

bool SaveArrayStringToFile(wxTextFile & txtFile,const wxArrayString &aryStr) 
{ 
    for(int i=0;i<aryStr.GetCount();i++) 
    { 
        txtFile.AddLine(aryStr.Item(i)); 
    } 
    return true; 
} 

int main( int argc, char** argv )
{
	// initialize wxWidgets
	wxInitializer init;
    wxArrayString files;
    wxTextFile txtFile(argv[1]); 
    txtFile.Create(); 
    
	wxPrintf( wxT("Hello in wxWidgets World!\n\n") );
    wxString dirPath = argv[0];
    printf("directory %s \n",dirPath.ToUTF8());
    wxString filename;
    size_t nbFiles =  wxDir::GetAllFiles(dirPath, &files);
    for(int i = 0;i < nbFiles;i++)
    {
        wxArrayString output;
        printf("%s\n", files[i]);
        wxString command = "otool -L " + files[i];
        wxExecute(command.c_str(), output);
        SaveArrayStringToFile(txtFile, output);
    }
    
    txtFile.Close(); 
	return 0;
}
