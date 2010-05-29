TODO: Insert fancy copyright

TODO: Insert authors

TODO: Insert program description


Commands line options:

--config=<file name>	specifies a file name for the config file
			defaults to config.ini


The application is configured using a config.ini file.
The format of the config.ini is as follows

[UserPreferences]
File=<user preferences file name>

[Logger]
Enabled=<0|1>
Format=<Human|Replay>
Output=<Console|File>
File=<log file name>

[Replayer]
Enabled=<0|1>
File=<replay file name>
ReplayNetwork=<0|1>
ReplayUI=<0|1>
ReplayFalcon=<0|1>
ReplayZCamera=<0|1>