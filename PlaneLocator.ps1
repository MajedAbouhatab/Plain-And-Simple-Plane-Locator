$AZPath="C:\Program Files (x86)\Microsoft Azure Sphere SDK\Tools\azsphere.exe"
$VSBinPath=$ENV:UserProfile+"\source\repos\PlainAndSimple\PlaneLocator\"
& $AZPath "dev" "sl" "delete"
& $AZPath "dev" "sl" "deploy" "-p"$VSBinPath"bin\ARM\Debug\PlaneLocator.imagepackage"