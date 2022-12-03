# Untethered Spotify DevTools

 C++ code to enable Spotify's Dev Tools, in an untethered manner.
 
 ## Usage
 Once, the Spotify executable is running, run the executable and it should patch the appropriate memory address ^_^.              
 This should grant the user access to the dev tools until the Spotify application is restarted / reloaded.
 
 ### Version - Compatibility
 `1.1.80+ - 1.2.0+`
 
 ### Tips - Advanced
 Using this method the right context menu, doesn't work like it used to which enabled the user to call the dev tools,
 as well as reload the application, albeit with the above method, the access to dev tools is lost once the application is restarted,
 since we patch the memory address holding the value for dev tools, but with the instructions below, if the `executable` is run,
 the right context menu gets enabled with the dev tools. Also, note this method isn't persistent after an update, since this method involves
 editing the `xpui.js` file found inside the `xpui.spa` file.
 
 Anyway to enable access to the right context menu, if you are familiar with [BlockTheSpot](https://github.com/mrpond/BlockTheSpot) 
 proceed, otherwise take some time to get acquainted with the repo's work.
 
 #### Steps
 1. You will need to download the powershell script from the BlockTheSpot Repo locally so that we can edit the script ourselves,
    in the repo, the script should be under the name `install.ps1`.
 2. Once, that is done, open the file in your favourite text editor and goto line number `326`
    to be precise the line should look like, 
    `$xpuiContents = $xpuiContents -replace '((?:"a"))\S+noopener nofollow.+?,.\)', '$1)'`.
 3. After which, we add the following line to the script,                                                   
    `$xpuiContents = $xpuiContents -replace '(isDeveloperMode|is_developer_mode)(:)(.*?)(,|})', '$1$2!0$4'`.
 6. That's it, save the script and run the powershell script.

 ## Acknowledgements
 * Initial Code Snippet: https://github.com/spicetify/spicetify-cli/issues/1518#issuecomment-1086363273
 * http://gnuwin32.sourceforge.net/version.c.txt
 
 Cheers - Exhigh
 
