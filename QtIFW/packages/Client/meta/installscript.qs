function Component()
{
 
    // default constructor
 
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
    component.addOperation("CreateShortcut", 
                            "@TargetDir@/CUITExamClient.exe",// target
                            "@DesktopDir@/成都信息工程大学区块链学院考试客户端.lnk",// link-path
                            "workingDirectory=@TargetDir@",// working-dir
                            "iconPath=@TargetDir@/CUITExamClient.exe", "iconId=0",// icon
                            "description=成都信息工程大学区块链学院考试客户端");// description
    component.addOperation("CreateShortcut", 
                            "@TargetDir@/CUITExamClient.exe",// target
                            "@StartMenuDir@/成都信息工程大学区块链学院考试客户端.lnk",// link-path
                            "workingDirectory=@TargetDir@",// working-dir
                            "iconPath=@TargetDir@/CUITExamClient.exe", "iconId=0",// icon
                            "description=成都信息工程大学区块链学院考试客户端");// description
    }
}