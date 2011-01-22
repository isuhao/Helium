#include "Platform/Environment.h"

bool Helium::GetUsername( tstring& username )
{
    return GetEnvironmentVariable( TXT( "USERNAME" ), username );
}

bool Helium::GetComputer( tstring& computername )
{
    return GetEnvironmentVariable( TXT( "COMPUTERNAME" ), computername );
}

bool Helium::GetPreferencesDirectory( tstring& preferencesDirectory )
{
    return Helium::GetEnvironmentVariable( TXT( "USERPROFILE" ), preferencesDirectory );
}

bool Helium::GetGameDataDirectory( tstring& gameDataDirectory )
{
    return Helium::GetEnvironmentVariable( TXT( "APPDATA" ), gameDataDirectory );
}