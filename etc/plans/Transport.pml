{
  "id" : 1573418869596,
  "name" : "Transport",
  "comment" : "",
  "relativeDirectory" : "",
  "variables" : [ ],
  "masterPlan" : false,
  "utilityThreshold" : 0.0,
  "preCondition" : null,
  "runtimeCondition" : null,
  "entryPoints" : [ {
    "id" : 1573418933317,
    "name" : "1573418933317",
    "comment" : "",
    "successRequired" : true,
    "minCardinality" : 0,
    "maxCardinality" : 20,
    "task" : "ServiceRobotsTasks.tsk#1555601323994",
    "state" : 1573418936395,
    "plan" : 1573418869596
  } ],
  "states" : [ {
    "type" : "State",
    "id" : 1573418936395,
    "name" : "Search",
    "comment" : "",
    "entryPoint" : 1573418933317,
    "parentPlan" : 1573418869596,
    "abstractPlans" : [ "Behaviours/Search.beh#1573419059418" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1573418996008, 1573419007368 ],
    "inTransitions" : [ ]
  }, {
    "type" : "State",
    "id" : 1573418937725,
    "name" : "PickUp",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1573418869596,
    "abstractPlans" : [ "Behaviours/Manipulate.beh#1571687572903" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1573418998111 ],
    "inTransitions" : [ 1573418996008, 1573419009128 ]
  }, {
    "type" : "State",
    "id" : 1573418938883,
    "name" : "Move",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1573418869596,
    "abstractPlans" : [ "Behaviours/Move.beh#1568825137528" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1573419000441 ],
    "inTransitions" : [ 1573418998111 ]
  }, {
    "type" : "State",
    "id" : 1573418939737,
    "name" : "PutDown",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1573418869596,
    "abstractPlans" : [ "Behaviours/Manipulate.beh#1571687572903" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1573419003625 ],
    "inTransitions" : [ 1573419000441 ]
  }, {
    "type" : "State",
    "id" : 1573418941076,
    "name" : "MoveClose",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1573418869596,
    "abstractPlans" : [ "Behaviours/Move.beh#1568825137528" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1573419009128 ],
    "inTransitions" : [ 1573419007368 ]
  }, {
    "type" : "TerminalState",
    "id" : 1573418943460,
    "name" : "TransportDone",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1573418869596,
    "abstractPlans" : [ ],
    "variableBindings" : [ ],
    "outTransitions" : [ ],
    "inTransitions" : [ 1573419003625 ],
    "success" : true,
    "postCondition" : null
  } ],
  "transitions" : [ {
    "id" : 1573418996008,
    "name" : "FromSearchToPickUp",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418936395,
    "outState" : 1573418937725,
    "preCondition" : {
      "id" : 1573419746851,
      "name" : "1573419746851",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Found & Close By",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1573418998111,
    "name" : "FromPickUpToMove",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418937725,
    "outState" : 1573418938883,
    "preCondition" : {
      "id" : 1573419810456,
      "name" : "1573419810456",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Picked up",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1573419000441,
    "name" : "FromMoveToPutDown",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418938883,
    "outState" : 1573418939737,
    "preCondition" : {
      "id" : 1573419828606,
      "name" : "1573419828606",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Moved to destination",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1573419003625,
    "name" : "FromPutDownToTransportDone",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418939737,
    "outState" : 1573418943460,
    "preCondition" : {
      "id" : 1573419841944,
      "name" : "1573419841944",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Object put down",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1573419007368,
    "name" : "FromSearchToMoveClose",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418936395,
    "outState" : 1573418941076,
    "preCondition" : {
      "id" : 1573419765536,
      "name" : "1573419765536",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Found & Too far away for grasping",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1573419009128,
    "name" : "FromMoveCloseToPickUp",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418941076,
    "outState" : 1573418937725,
    "preCondition" : {
      "id" : 1573419800282,
      "name" : "1573419800282",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Found & Close by",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  } ],
  "synchronisations" : [ ]
}