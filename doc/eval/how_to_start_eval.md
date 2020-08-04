# How To Start the Evaluation

There are different setups for the evaluation, each one with a different set of processes to start and parameters to set. First lets explain a little bit on all of the processes there are.

**Control Panel**

The control panel is the UI which allows to start and stop the agents, as well as sending commands to the agents.![ControlPanelUI](/home/emmeda/Thesis/srgws/src/service-robot-ai/doc/eval/img/ControlPanelUI.png)

The control panel located in the service-robot-ai repository and can be started by executing the following command: `rosrun control_panel control_panel`

**SRG Base**

The SRG Base is the actual AI or agent process. It is located in the service-robot-ai repository and can be started with the following command: `ROBOT=<name> rosrun srg_base srg_base -m ServeMaster`

The value for the ROBOT environment variable needs to be chosen from the service-robot-ai/etc/Globals.conf file. There are different agents listed, but all except the homepc have the role *Assistant*. The homepc agent has the role *Human*. You need to spawn atleast one human, in order to generate the tasks for the evaluation.

**Grid Simulator**

The grid simulator is the simulator for the service robot world. Its source code is located in the [service-robot-grid-simulation repository](https://github.com/dasys-lab/service-robot-grid-simulation) and its package name is *grid_sim*. It can be spawned with: `rosrun grid_sim grid_sim`

Further it supports the optional command line argument  `--headless` in order to spawn it without UI, which will decrease its resource consumption a little.

**CN5 Database**

The CN5 Database is only accessible through a web server that needs to be spawned, so that all service robots have access to it during the evaluation. It can be started with: 

`cd conceptnet5/`

`python3 web/conceptnet_web/api.py`

## Evaluation Scenarios

For all the evaluation scenarios you need to start the control panel, the grid simulator, and the Concept Net web server:

`rosrun control_panel control_panel `

`rosrun grid_sim grid_sim`

`python3 web/conceptnet_web/api.py`

### Single Robot Scenarios

In all the different single robot scenarios you will need to start one SRG Base process for the human and one SRG Base process for the service robot:

`ROBOT=homepc rosrun srg_base srg_base -m ServeMaster`

`ROBOT=donatello rosrun srg_base srg_base -m ServeMaster`

In order to distinguish between with commensense and without commensense, you will need to modify and recompile the ObjectSearch.cpp file in the srg_agent package. In the init(..) method you need to comment the initFringeWithProbableLocations() method.

