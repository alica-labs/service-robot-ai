{
  "id" : 1555601736192,
  "name" : "ServeMaster",
  "comment" : "",
  "relativeDirectory" : "",
  "variables" : [ ],
  "masterPlan" : true,
  "utilityThreshold" : 0.0,
  "preCondition" : {
    "id" : 1558533725689,
    "name" : "1558533725689",
    "comment" : "",
    "enabled" : true,
    "conditionString" : "Stop received",
    "pluginName" : "DefaultPlugin",
    "variables" : [ ],
    "quantifiers" : [ ]
  },
  "runtimeCondition" : null,
  "entryPoints" : [ {
    "id" : 1555601746711,
    "name" : "1555601746711",
    "comment" : "",
    "successRequired" : false,
    "minCardinality" : 0,
    "maxCardinality" : 20,
    "task" : "ServiceRobotsTasks.tsk#1555601344076",
    "state" : 1555601748848,
    "plan" : 1555601736192
  } ],
  "states" : [ {
    "type" : "State",
    "id" : 1555601748848,
    "name" : "Stop",
    "comment" : "",
    "entryPoint" : 1555601746711,
    "parentPlan" : 1555601736192,
    "abstractPlans" : [ "Behaviours/Stop.beh#1555602210283" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1558533492085 ],
    "inTransitions" : [ 1558533494595, 1558533714287 ]
  }, {
    "type" : "State",
    "id" : 1558533460297,
    "name" : "Spawn",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1555601736192,
    "abstractPlans" : [ "Behaviours/Spawn.beh#1558533534825" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1558533494595, 1558533514929 ],
    "inTransitions" : [ 1558533492085, 1558533519854 ]
  }, {
    "type" : "State",
    "id" : 1558533461982,
    "name" : "Serve",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1555601736192,
    "abstractPlans" : [ "Behaviours/Test.beh#1559635479527" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1558533498560, 1558533519854, 1558533714287 ],
    "inTransitions" : [ 1558533498560, 1558533514929 ]
  } ],
  "transitions" : [ {
    "id" : 1558533492085,
    "name" : "FromStopToSpawn",
    "comment" : "MISSING_COMMENT",
    "inState" : 1555601748848,
    "outState" : 1558533460297,
    "preCondition" : {
      "id" : 1558533620473,
      "name" : "1558533620473",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Start received",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1558533494595,
    "name" : "FromSpawnToStop",
    "comment" : "MISSING_COMMENT",
    "inState" : 1558533460297,
    "outState" : 1555601748848,
    "preCondition" : {
      "id" : 1558533654035,
      "name" : "1558533654035",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Stop received",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1558533498560,
    "name" : "FromServeToServe",
    "comment" : "MISSING_COMMENT",
    "inState" : 1558533461982,
    "outState" : 1558533461982,
    "preCondition" : null,
    "synchronisation" : null
  }, {
    "id" : 1558533514929,
    "name" : "FromSpawnToServe",
    "comment" : "MISSING_COMMENT",
    "inState" : 1558533460297,
    "outState" : 1558533461982,
    "preCondition" : {
      "id" : 1558533667052,
      "name" : "1558533667052",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "Spawn successful",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1558533519854,
    "name" : "FromServeToSpawn",
    "comment" : "MISSING_COMMENT",
    "inState" : 1558533461982,
    "outState" : 1558533460297,
    "preCondition" : null,
    "synchronisation" : null
  }, {
    "id" : 1558533714287,
    "name" : "FromServeToStop",
    "comment" : "MISSING_COMMENT",
    "inState" : 1558533461982,
    "outState" : 1555601748848,
    "preCondition" : null,
    "synchronisation" : null
  } ],
  "synchronisations" : [ ]
}