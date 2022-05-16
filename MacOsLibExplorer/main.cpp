#include <wx/init.h>
#include <wx/utils.h> 
#include <wx/dir.h> 
#include <string>
#include <wx/filename.h>
#if wxVERSION_NUMBER < 2900
    #include <wx/string.h>
#else
    #include <wx/crt.h>
#endif
#include <wx/textfile.h>
#include <map>
#include <iostream>
using namespace std;

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

wxArrayString ExecuteProcess(wxArrayString files, int nbFiles, bool isMacOsFolder, wxString folder_output_final, wxString fileOutToWrite)
{
    
    wxRemove(fileOutToWrite);
    wxTextFile txtFile(fileOutToWrite); 
    txtFile.Create(); 
    txtFile.Open();
   std::map<wxString, wxString> copyFile;
   std::vector<wxString> changeNameTool;
   for(int i = 0;i < nbFiles;i++)
    {
        wxArrayString output;
        printf("File : %s\n", files[i].ToStdString().c_str());
        wxString command = "otool -L " + files[i];
        wxExecute(command.c_str(), output);
        
        wxString fileName = files[i];
        std::vector<wxString> libName = split(files[i], '/');
        wxString dylibName = libName[libName.size() - 1];
        int start = isMacOsFolder ? 1 : 2;
         bool findModification = false;

        for(int i=start;i<output.GetCount();i++) 
        { 
            wxString lineText = output.Item(i);
            

            if(!(lineText.find("/System/Library/Frameworks") == 1 || lineText.find("/usr/lib") == 1 || lineText.find("@executable_path") == 1))
            {
                wxPrintf("lineText Lib : " + lineText + "\n");
                std::vector<wxString> listOfString = split(lineText, '(');
                wxString toWrite = "cp ";
                 wxString libPath = listOfString[0];
                 std::vector<wxString> listOflib = split(libPath, '/');
                if(lineText.find("@loader_path") == 1 || lineText.find("@rpath") == 1)
                {
                     toWrite += "/opt/homebrew/lib/" + listOflib[listOflib.size() - 1];
                     libPath = listOflib[listOflib.size() - 1];
                }
                else
                {
                    toWrite += libPath;
                }
                
                wxString outputFilename = libPath;
                outputFilename = listOflib[listOflib.size() - 1];
                outputFilename.Replace("-3.1.6.0.0.dylib", "-3.1.dylib");
                wxString outputLib = folder_output_final + "/" + outputFilename;

                 if(dylibName == outputFilename)
                     continue;
                
                wxString fileToTest = folder_output_final + "/" + outputFilename;
                if(wxFileExists(fileToTest) == TRUE)
                {
                      //printf("File exists  \n");
                      continue;
                }

                if(!wxFileExists(fileToTest) && libPath.find("3.1.6.0.0.dylib")==wxNOT_FOUND)
                {
                   // printf("copyFile \n");
                    copyFile[outputFilename] = toWrite + " " + fileToTest;
                }

                wxString installName = "install_name_tool -change " + libPath + " @executable_path/../Frameworks/" + outputFilename + " " + fileName;
                //int pos = lineText.find("@rpath");
                //printf("Pos : %s + \n",  std::to_string(pos).c_str());
                
                if(lineText.find("@rpath") != -1)
                {
                   installName = "install_name_tool -change @rpath/" + outputFilename + " @executable_path/../Frameworks/" + outputFilename + " " + fileName;
                  // printf("add install name \n");
                    
                }
                    
                changeNameTool.push_back(installName);
                findModification = true;
                
            }
        } 
    }
    
    wxArrayString filesOut;
    int nbLine = 0;
    std::map<wxString, wxString>::iterator it = copyFile.begin();
    while(it != copyFile.end())
    {
        auto value = it->second;
        txtFile.AddLine(value); 
        it++;
        filesOut.push_back(folder_output_final + "/" + it->first);
        //nbLine++;
    }

    for(wxString installName : changeNameTool) 
    { 
        txtFile.AddLine(installName); 
        //wxPrintf(installName +  "\n");
        //wxExecute(installName);
        //nbLine++;
    } 
    
    txtFile.Write();
    txtFile.Close(); 

    printf("Execute modif lib \n");
    wxArrayString output;
    wxExecute("./modif_lib.sh", output);

    return filesOut;
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
    

    
	wxPrintf( wxT("Hello in wxWidgets World!\n\n") );
    wxString dirPath = argv[1];
    std::vector<wxString> libNameFolder = split(dirPath, '/');
    wxString folder_output_final = argv[2];
   // wxString folder_output = "./Release/RegardsViewer.app/Contents/Frameworks";
    
    printf("directory %s \n",dirPath.ToStdString().c_str());
    wxString filename;
    size_t nbFiles =  wxDir::GetAllFiles(dirPath, &files);
    do
    {
        files = ExecuteProcess(files, nbFiles, isMacOsFolder, folder_output_final, argv[3]);
        nbFiles = files.size();
        isMacOsFolder = false;
    }while(nbFiles > 0);
    
    

	return 0;
}
