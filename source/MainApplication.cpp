#include <MainApplication.hpp>
#include <stdio.h>
#include <sys/stat.h>
// Implement all the layout/application functions here
#include <FileHandler.h>
int val;
int backReserve = 1;
nxPathData pathData;
FileMode currentFileMode = FileMode::DEFAULT;
pu::ui::elm::MenuItem::Ref selectedItem;
std::string  iname;
pu::ui::elm::MenuItem::Ref MakeButton(char name[256], int num)
{
    //Open current-working-directory.
    strcpy(&dirData.dirName[val][0],name);
    pu::ui::Color red(255,0, 0, 255 );		
    pu::ui::Color white(255, 255,255, 255);		
    pu::ui::elm::MenuItem::Ref buttonMenuItem;
    buttonMenuItem =  pu::ui::elm::MenuItem::New(name);	
    int value = val;
    auto button_callback = [value]() {
        //handle if file or dir
        //set as seleted item for iteration
       // Open Dir pathdata
       
        strcpy(&currentDir,dirData.dirName[value]);
        strcpy(&pathData.fileNames[pathData.fileCount][0] ,&currentDir);
        pathData.fileCount++;

        if(dirData.filetype[val][0] == 0)
        {
            
        }
        else
        {
            //send in file to load

        }
        appStarted = false;

        
    };

    buttonMenuItem->AddOnKey(button_callback);

    val++;
    return buttonMenuItem;
}
CustomLayout::CustomLayout() : Layout::Layout() {
    // Create the TextBlock instance with the text we want
    //add logic here!
    pu::ui::Color red(255,0, 0, 255 );		
    pu::ui::Color white(255, 255,255, 255);
    pu::ui::elm::MenuItem::Ref bbuttonMenuItem;
    bbuttonMenuItem =  pu::ui::elm::MenuItem::New("..");	
    auto button_callback_back = []() {
        DIR* tempdir;
        tempdir = opendir("..");//Open current-working-directory.
        if(tempdir!=NULL)
        {
           pathData.fileCount--;
            if(pathData.fileCount < backReserve){

                pathData.fileCount = backReserve;//never write to zero
                strcpy(&currentDir,"..");
                strcpy(&pathData.fileNames[pathData.fileCount][0] ,&currentDir);
                pathData.fileCount++;
                backReserve++;

            }
            else{
                  strcpy(&currentDir,"..");
            }
           
            closedir(tempdir);


            appStarted = false;
        }
        
    };
    bbuttonMenuItem->AddOnKey(button_callback_back);
    directoryMenu =  pu::ui::elm::Menu::New(300, 50, 900, white, red, 100, 5);
    directoryMenu->AddItem(bbuttonMenuItem);
    char dirText[256];
    char text[600];
    DIR* dir;
    struct dirent* ent;
    struct stat st;
    dir = opendir(&currentDir);//Open current-working-directory.
    if(dir!=NULL)
    {
        while ((ent = readdir(dir)))
        {
            strncpy(dirText,ent->d_name, sizeof(ent->d_name));
            pu::ui::elm::MenuItem::Ref menutItem = MakeButton(dirText, val);
            directoryMenu->AddItem(menutItem);
           
        }
        closedir(dir);
        
        //sprintf(text, "Directory: %s",  pathData.fileNames[pathData.fileCount]);
        get_current_directory();
        sprintf(text, "Directory: %s",  &pathData.pathTot);
    }
    else
    {
        get_current_directory();

        sprintf(text, "Directory Empty: %s",  &pathData.pathTot);

    }
        this->Add(this->directoryMenu);
        chdir(&currentDir);
        this->helloText = pu::ui::elm::TextBlock::New(300, 0, text);
        // Add the instance to the layout. IMPORTANT! this MUST be done for them to be used, having them as members is not enough (just a simple way to keep them)
        this->Add(this->helloText);
//Later -> launching NRO, install NSP, Launch NCA
//BACK BUTTON
  
 	
    


  /* pu::ui::elm::Button::Ref backButton;
    backButton = pu::ui::elm::Button::New(0, 600, 130, 130,  "Go Back" ,red, white);
    auto button_callback_back = []() {
        DIR* dirTemp;
        dirTemp = opendir("..");//Open current-working-directory.
        if(dirTemp != NULL){
            pathData.fileCount--;
            strcpy(&currentDir,"..");
            appStarted = false;
        }
        
    };
    backButton->SetOnClick(button_callback_back);
    Add(backButton);*/

//CONFIRM
if(CurrentFileMode() == FileMode::MOVE){
    pu::ui::elm::Button::Ref cutButton;
    cutButton = pu::ui::elm::Button::New(150, 600, 130, 130,  "Confirm" ,red, white);
    auto conbutton_callback_back = []() {
        //for now just change the file mode
      
        SetFileMode(FileMode::DEFAULT);
        appStarted = false;

    };
    cutButton->SetOnClick(conbutton_callback_back);
    Add(cutButton);
}
//Make Folder
if(CurrentFileMode() == FileMode::DEFAULT){
    pu::ui::elm::Button::Ref mkDirButton;
    //Button (const i32 x, const i32 y, const i32 width, const i32 height, const std::string &content, const Color content_clr, const Color bg_clr)
    mkDirButton = pu::ui::elm::Button::New(0, 600, 200, 130,  "New Folder" ,red, white);
    auto conbutton_callback_back = []() {
        //for now just change the file mode
        //get input and assign it
        SwkbdConfig keyboard_config;
        swkbdCreate(&keyboard_config, 0);
        swkbdConfigMakePresetDefault(&keyboard_config);
        swkbdConfigSetGuideText(&keyboard_config, "Enter your name:");
        char input_buffer[256];
        swkbdShow(&keyboard_config, input_buffer, sizeof(input_buffer));
           // printf("You entered: %s\n", input_buffer);
        if(input_buffer != nullptr){
        MkDirWithName(input_buffer);
        appStarted = false;
        }
       
        //mkdir 
        appStarted = false;

    };
    mkDirButton->SetOnClick(conbutton_callback_back);
    Add(mkDirButton);
}

}
//call before load
void MainApplication::AppLoop()//create more custom dialogs based on events act as debug
{
  
        
            dialog =  pu::ui::Dialog::New("Notice", "Dir is Empty");
            dialog->AddOption("OK");
            this->ShowDialog(dialog);// If we will ignore the option, it doesn't matter if this is true or false
    
}
void MainApplication::OnLoad() {
    // Create the layout (calling the smart constructor above)
   
    

    this->layout = CustomLayout::New();

    // Load the layout. In applications layouts are loaded, not added into a container (you don't select an added layout, just load it from this function)
    // Simply explained: loading layout = the application will render that layout in the very next frame
    this->LoadLayout(this->layout);
    // Set a function when input is caught. This input handling will be the first one to be handled (before Layout or any Elements)
    // Using a lambda function here to simplify things
    // You can use member functions via std::bind() C++ wrapper
    this->SetOnInput([&](const u64 keys_down, const u64 keys_up, const u64 keys_held, const pu::ui::TouchPoint touch_pos) {
         // If X is pressed, start with our dialog questions!
        //press X to open edit menu
        //if in edit and menu item changes
                //cancel edit
        if(!appStarted)
        {
            this->Close();
        }
        if(showMadeDiag){
                this->CreateShowDialog(iname, "Directory is Empty ", { "Ok"}, true);
        }
        //add to path stack
        pu::ui::elm::MenuItem::Ref curSelectedItem =  this->layout->directoryMenu->GetSelectedItem();
        char pathMinusOne;
        char pathFull;
        if(keys_down & HidNpadButton_X) {
            
            //Check filemode to handle input
            if(CurrentFileMode() == FileMode::DEFAULT)
            {
                iname = this->layout->directoryMenu->GetSelectedItem()->GetName();
                selectedItem =  this->layout->directoryMenu->GetSelectedItem();//set selected item for modification
                //INIT DIALOG FOR EDIT
                int opt = this->CreateShowDialog(iname, "Modify: ", { "Cut", "Copy", "Rename", "Delete", "Cancel" }, true); // (using latest option as cancel option)
                // -1 and -2 are similar, but if the user cancels manually -1 is set, other types or cancel should be -2.
                if(opt == 4)  {
                    this->CreateShowDialog("Cancel", "Canceled Options", { "Ok" }, true); // If we will ignore the option, it doesn't matter if this is true or false
                }
                else {
                    // Otherwise, opt will be the index of the options we passed to the dialog
                    pathFull = pathData.pathTot;
                    //pathMinusOne
                    switch(opt) {
                        // "Yes!" was selected
                        case 0:  {
                            //Set Mode to Move by Default
                            SetFileMode(FileMode::MOVE);
                            strncpy(pathData.fileNames[pathData.fileCount], iname.c_str(), 255);
                            pathData.fileNames[pathData.fileCount][255] = '\0';
                            this->CreateShowDialog(pathData.fileNames[pathData.fileCount], "Cut", { "Ok" }, true); // Same here ^
                            pathData.fileCount++;
                            appStarted = false;//reset scene
                            break;
                        }
                        // "No..." was selected
                        case 1: {
                            //Set Mode to Move by Default
                            SetFileMode(FileMode::MOVE);
                            strncpy(pathData.fileNames[pathData.fileCount], iname.c_str(), 255);
                            pathData.fileNames[pathData.fileCount][255] = '\0';
                            this->CreateShowDialog(pathData.fileNames[pathData.fileCount], "Copy", { "Ok" }, true); // And here ^
                            pathData.fileCount++;
                            appStarted = false;//reset scene
                            break;
                        }
                          case 2: {
                            //Set Mode to Move by Default
                            //SetFileMode(FileMode::MOVE); special case rename and be done
                            strncpy(pathData.fileNames[pathData.fileCount], iname.c_str(), 255);
                            pathData.fileNames[pathData.fileCount][255] = '\0';
                            this->CreateShowDialog(pathData.fileNames[pathData.fileCount], "Rename", { "Ok" }, true); // And here ^
                            appStarted = false;//reset scene
                            break;
                        }
                          case 3: {
                            //Set Mode to Move by Default
                            //SetFileMode(FileMode::MOVE); special Case, delete and be done
                            strncpy(pathData.fileNames[pathData.fileCount], iname.c_str(), 255);
                            pathData.fileNames[pathData.fileCount][255] = '\0';
                            this->CreateShowDialog(pathData.fileNames[pathData.fileCount], "Delete", { "Ok" }, true); // And here ^
                            pathData.fileCount++;
                            appStarted = false;//reset scene
                            break;
                        }
                    }
                }
            }
            else if(CurrentFileMode() == FileMode::MOVE)
            {
                
                int opt = this->CreateShowDialog(iname, "Cancel Modify? ", { "Ok", "Cancel" }, true); // (using latest option as cancel option)
                // -1 and -2 are similar, but if the user cancels manually -1 is set, other types or cancel should be -2.
                if(opt == 1)  {
                    SetFileMode(FileMode::MOVE);
                    this->CreateShowDialog("Canceled Modify", "", { "Ok" }, true); // Same here ^
                    appStarted = false;//reset scene          }
            }
           
        }
        // If + is pressed, exit application
        //else 
        if(keys_down & HidNpadButton_Plus) {
            endApp = true;
            this->Close();
        }
      
    }
        //handle file controls here by taking var input ex: 0-cut 1-copy 2-rename 3-delete
        //Get selectedItem and get details or set a path var : dest, source path vars
    });
            

}