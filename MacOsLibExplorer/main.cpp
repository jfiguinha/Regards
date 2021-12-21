#include <wx/init.h>
#include <wx/utils.h> 
#include <wx/dir.h> 
#if wxVERSION_NUMBER < 2900
    #include <wx/string.h>
#else
    #include <wx/crt.h>
#endif
#include <wx/textfile.h>
#include <map>

bool SaveArrayStringToFile(wxTextFile & txtFile,const wxArrayString &aryStr) 
{ 
    for(int i=0;i<aryStr.GetCount();i++) 
    { 
       // txtFile.AddLine(aryStr.Item(i)); 
    } 
    return true; 
} 

std::vector<wxString> split(const wxString& s, char seperator)
{
	std::vector<wxString> output;

	wxString::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		wxString substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}

int main( int argc, char** argv )
{
    bool isMacOsFolder = true;
    if(argc < 3)
    {
        printf("Missing folder argument");
        return 0;
    }
    
    if(argc == 4)
        isMacOsFolder = false;
	// initialize wxWidgets
	wxInitializer init;
    wxArrayString files;
    wxTextFile txtFile(argv[3]); 
    txtFile.Create(); 
    txtFile.Open();
    
    std::map<wxString, wxString> copyFile;
    std::vector<wxString> changeNameTool;
    std::map<wxString, wxString> moveFile;
    
	wxPrintf( wxT("Hello in wxWidgets World!\n\n") );
    wxString dirPath = argv[1];
    std::vector<wxString> libNameFolder = split(dirPath, '/');
    wxString folder_output_final = argv[2];
    wxString folder_output = "./Frameworks";
    
    printf("directory %s \n",dirPath.ToStdString().c_str());
    wxString filename;
    size_t nbFiles =  wxDir::GetAllFiles(dirPath, &files);
    for(int i = 0;i < nbFiles;i++)
    {
        wxArrayString output;
        printf("File : %s\n", files[i].ToStdString().c_str());
        wxString command = "otool -L " + files[i];
        wxExecute(command.c_str(), output);
        
        wxString fileName = files[i];
        std::vector<wxString> libName = split(files[i], '/');
        wxString dylibName = libName[libName.size() - 1];
        //SaveArrayStringToFile(txtFile, output);
        int start = isMacOsFolder ? 1 : 2;
         bool findModification = false;

        for(int i=start;i<output.GetCount();i++) 
        { 
            wxString lineText = output.Item(i);
            //int index = lineText.find("/System/Library/Frameworks");
            
            //|| lineText.find(dylibName) > 1
            
            if(!(lineText.find("/System/Library/Frameworks") == 1 || lineText.find("/usr/lib") == 1 || lineText.find("@executable_path") == 1))
            {
                printf("Line %s\n", lineText.ToStdString().c_str());
                std::vector<wxString> listOfString = split(lineText, '(');
                wxString toWrite = "cp ";
                 wxString libPath = listOfString[0];
                if(lineText.find("@loader_path") == 1 || lineText.find("@rpath") == 1)
                {
                     std::vector<wxString> listOflib = split(libPath, '/');
                     toWrite += "/opt/homebrew/lib/" + listOflib[listOflib.size() - 1];
                }
                else
                    toWrite += libPath;
                folder_output.Replace("-3.1.5.0.0.dylib", "-3.1.dylib");
                toWrite += " " + folder_output;

                //txtFile.AddLine(toWrite);    
                 std::vector<wxString> listOflib = split(libPath, '/');
                wxString fileOut = folder_output_final + "/" + listOflib[listOflib.size() - 1];
                //printf("Copy file  : %s \n",toWrite.ToStdString().c_str());
                //printf("Test if file exist : %s \n",fileOut.ToStdString().c_str());
                if(!wxFileExists(fileOut))
                    copyFile[libPath] = toWrite;
                
                wxString outputFilename = listOflib[listOflib.size() - 1];
                outputFilename.Replace("-3.1.5.0.0.dylib", "-3.1.dylib");
                wxString installName = "install_name_tool -change " + libPath + " @executable_path/../Frameworks/" + outputFilename + " " + fileName;
                //txtFile.AddLine(installName); 
                changeNameTool.push_back(installName);
                findModification = true;
                
            }
        } 
         if(!isMacOsFolder && !findModification)
         {
            wxString toWrite = "mv ";
            toWrite += fileName;
            toWrite += " " + folder_output_final;
            moveFile[dylibName] = toWrite;
         }

    }
    
    int nbLine = 0;
    std::map<wxString, wxString>::iterator it = copyFile.begin();
    while(it != copyFile.end())
    {
        auto value = it->second;
        txtFile.AddLine(value); 
        it++;
        nbLine++;
    }
    
    for(wxString installName : changeNameTool) 
    { 
        txtFile.AddLine(installName); 
        nbLine++;
    } 
    
    std::map<wxString, wxString>::iterator it_move = moveFile.begin();
    while(it_move != moveFile.end())
    {
        auto value = it_move->second;
        txtFile.AddLine(value); 
        nbLine++;
        it_move++;
    } 
    txtFile.Write();
    txtFile.Close(); 
    if(nbLine == 0)
        wxRemoveFile(argv[2]);
	return 0;
}
