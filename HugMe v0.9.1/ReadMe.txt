OfficeSlingshot3D
Haptic game that uses a novint falcon, tactile clothing and a Z-Camera to provide enhanced user interaction.
Copyright (C) 2010 Kevin Mottashed, Philippe Doré, Daniel Grisé, Benoit Renaud and Alexandre Bradley

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.






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