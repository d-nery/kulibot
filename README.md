# Kulibot
Simple single server bot for my D&D RPG group

----

This bot only works with slash commands, on commands folder:

* Ping: Just checks if it is working
* boh: Bag of Holding commands (inventory)
* char: Shows information about our characters

----

The bot folder contains the main bot class (derived from dpp::cluster) and the abstract command definition

The utils folder contains global access to data (config, inventory, characters, etc...)

All image URL and custom emoji are hardcoded in the config file (too lazy to fetch them from discord and send attachments)

----

Tests can be built by changing the COMPILE_TEST variable to True in CMakeLists.txt and ran with `./kuliteste` on the build folder. Not everything is tested, mostly utils
