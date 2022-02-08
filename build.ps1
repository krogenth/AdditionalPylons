Set-Location ./libs/bwapi

if(-not(Test-Path -Path 'bwapi/Debug')) {
    msbuild /m /verbosity:detailed /p:Configuration=Debug_Pipeline ./bwapi/bwapi.sln
    msbuild /m /verbosity:detailed /p:Configuration=Release_Pipeline ./bwapi/bwapi.sln
}

Set-Location ../../
#
msbuild /m /verbosity:detailed /p:Configuration=Debug /p:Platform=x86 ./vs/AdditionalPylons.sln
if(!$?) {
    throw "Failed to build AdditionalPylons Debug"
}
msbuild /m /verbosity:detailed /p:Configuration=Release /p:Platform=x86 ./vs/AdditionalPylons.sln
if(!$?) {
    throw "Failed to build AdditionalPylons Release"
}