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
    wxTextFile txtFile(argv[2]); 
    txtFile.Create(); 
    
	wxPrintf( wxT("Hello in wxWidgets World!\n\n") );
    wxString dirPath = argv[1];
    printf("directory %s \n",dirPath.ToStdString().c_str());
    wxString filename;
    size_t nbFiles =  wxDir::GetAllFiles(dirPath, &files);
    for(int i = 0;i < nbFiles;i++)
    {
        wxArrayString output;
        printf("File : %s\n", files[i].ToStdString().c_str());
        wxString command = "otool -L " + files[i];
        wxExecute(command.c_str(), output);
        //SaveArrayStringToFile(txtFile, output);
        for(int i=2;i<output.GetCount();i++) 
        { 
            wxString lineText = output.Item(i);
            //int index = lineText.find("/System/Library/Frameworks");
             //printf("index %i \n", index);
            
            if(!(lineText.find("/System/Library/Frameworks") == 1 || lineText.find("/usr/lib") == 1 || lineText.find("@executable_path") == 1))
            {
                printf("Line %s\n", lineText.ToStdString().c_str());
                txtFile.AddLine(lineText);                
            }

        } 
    }
    
    txtFile.Close(); 
	return 0;
}
