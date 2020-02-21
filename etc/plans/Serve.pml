{
  "id" : 1568825275605,
  "name" : "Serve",
  "comment" : "",
  "relativeDirectory" : "",
  "variables" : [ ],
  "masterPlan" : false,
  "utilityThreshold" : 0.0,
  "preCondition" : null,
  "runtimeCondition" : null,
  "entryPoints" : [ {
    "id" : 1568825285315,
    "name" : "1568825285315",
    "comment" : "",
    "successRequired" : false,
    "minCardinality" : 0,
    "maxCardinality" : 20,
    "task" : "ServiceRobotsTasks.tsk#1555601344076",
    "state" : 1568825288640,
    "plan" : 1568825275605
  } ],
  "states" : [ {
    "type" : "State",
    "id" : 1568825288640,
    "name" : "WaitForTask",
    "comment" : "",
    "entryPoint" : 1568825285315,
    "parentPlan" : 1568825275605,
    "abstractPlans" : [ "Behaviours/Stop.beh#1555602210283" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1568825336792, 1571661739802, 1573418725423, 1581966904890 ],
    "inTransitions" : [ 1568825392354, 1571661809581, 1573418821209, 1581966925392 ]
  }, {
    "type" : "State",
    "id" : 1568825309813,
    "name" : "MoveToPosition",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1568825275605,
    "abstractPlans" : [ "Behaviours/Move.beh#1568825137528" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1568825392354 ],
    "inTransitions" : [ 1568825336792 ]
  }, {
    "type" : "State",
    "id" : 1571661663929,
    "name" : "ManipulateObject",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1568825275605,
    "abstractPlans" : [ "Behaviours/Manipulate.beh#1571687572903" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1571661809581 ],
    "inTransitions" : [ 1571661739802 ]
  }, {
    "type" : "State",
    "id" : 1573418710533,
    "name" : "TransportToPosition",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1568825275605,
    "abstractPlans" : [ "Transport.pml#1573418869596" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1573418821209 ],
    "inTransitions" : [ 1573418725423 ]
  }, {
    "type" : "State",
    "id" : 1581966845202,
    "name" : "AnswerQuestion",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1568825275605,
    "abstractPlans" : [ "Behaviours/Answer.beh#1581966891046" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1581966925392 ],
    "inTransitions" : [ 1581966904890 ]
  } ],
  "transitions" : [ {
    "id" : 1568825336792,
    "name" : "FromWaitForTaskToMoveToPosition",
    "comment" : "MISSING_COMMENT",
    "inState" : 1568825288640,
    "outState" : 1568825309813,
    "preCondition" : {
      "id" : 1568825457853,
      "name" : "1568825457853",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "MovingTask received!",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1568825392354,
    "name" : "FromMoveToPositionToWaitForTask",
    "comment" : "MISSING_COMMENT",
    "inState" : 1568825309813,
    "outState" : 1568825288640,
    "preCondition" : {
      "id" : 1568825476581,
      "name" : "1568825476581",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Movement successful!",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1571661739802,
    "name" : "FromWaitForTaskToOpenDoor",
    "comment" : "MISSING_COMMENT",
    "inState" : 1568825288640,
    "outState" : 1571661663929,
    "preCondition" : {
      "id" : 1571661980674,
      "name" : "1571661980674",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Manipulate command!",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1571661809581,
    "name" : "FromOpenDoorToWaitForTask",
    "comment" : "MISSING_COMMENT",
    "inState" : 1571661663929,
    "outState" : 1568825288640,
    "preCondition" : {
      "id" : 1571661864299,
      "name" : "1571661864299",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Manipulation successful!",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1573418725423,
    "name" : "FromWaitForTaskToBringToPosition",
    "comment" : "MISSING_COMMENT",
    "inState" : 1568825288640,
    "outState" : 1573418710533,
    "preCondition" : {
      "id" : 1573418732991,
      "name" : "1573418732991",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Received a transport task!",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1573418821209,
    "name" : "FromBringToPositionToWaitForTask",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418710533,
    "outState" : 1568825288640,
    "preCondition" : {
      "id" : 1573418838905,
      "name" : "1573418838905",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Any children success",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1581966904890,
    "name" : "FromWaitForTaskToAnswerQuestion",
    "comment" : "MISSING_COMMENT",
    "inState" : 1568825288640,
    "outState" : 1581966845202,
    "preCondition" : {
      "id" : 1581966904893,
      "name" : "1581966904893",
      "comment" : "",
      "enabled" : true,
      "conditionString" : null,
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1581966925392,
    "name" : "FromAnswerQuestionToWaitForTask",
    "comment" : "MISSING_COMMENT",
    "inState" : 1581966845202,
    "outState" : 1568825288640,
    "preCondition" : {
      "id" : 1581966925394,
      "name" : "1581966925394",
      "comment" : "",
      "enabled" : true,
      "conditionString" : null,
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  } ],
  "synchronisations" : [ ]
}