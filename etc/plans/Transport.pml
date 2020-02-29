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
    "successRequired" : false,
    "minCardinality" : 1,
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
    "outTransitions" : [ 1573419007368 ],
    "inTransitions" : [ 1582991323347, 1582991345018 ]
  }, {
    "type" : "State",
    "id" : 1573418937725,
    "name" : "PickUp",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1573418869596,
    "abstractPlans" : [ "Behaviours/Manipulate.beh#1571687572903" ],
    "variableBindings" : [ ],
    "outTransitions" : [ 1573418998111, 1582991345018 ],
    "inTransitions" : [ 1573419009128 ]
  }, {
    "type" : "State",
    "id" : 1573418938883,
    "name" : "MoveDestination",
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
    "outTransitions" : [ 1583008562199 ],
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
    "outTransitions" : [ 1573419009128, 1582991323347 ],
    "inTransitions" : [ 1573419007368 ]
  }, {
    "type" : "State",
    "id" : 1583008553235,
    "name" : "PseudoSuccess",
    "comment" : "",
    "entryPoint" : null,
    "parentPlan" : 1573418869596,
    "abstractPlans" : [ ],
    "variableBindings" : [ ],
    "outTransitions" : [ ],
    "inTransitions" : [ 1583008562199 ]
  } ],
  "transitions" : [ {
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
      "conditionString" : "Object picked up, now bring it somewhere!",
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
      "conditionString" : "Object carried to destination, now put it down.",
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
      "conditionString" : "Object found, now move to it!",
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
      "conditionString" : "At the object, now pick it up!",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1582991323347,
    "name" : "FromMoveCloseTo Search",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418941076,
    "outState" : 1573418936395,
    "preCondition" : {
      "id" : 1582991323350,
      "name" : "1582991323350",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "MoveClose does not work anymore, because its target object vanished! Go back to Search.",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1582991345018,
    "name" : "FromPickUpTo Search",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418937725,
    "outState" : 1573418936395,
    "preCondition" : {
      "id" : 1582991345020,
      "name" : "1582991345020",
      "comment" : "",
      "enabled" : true,
      "conditionString" : "PickUp is not executable, because its object vanished. Go back to Search!",
      "pluginName" : "DefaultPlugin",
      "variables" : [ ],
      "quantifiers" : [ ]
    },
    "synchronisation" : null
  }, {
    "id" : 1583008562199,
    "name" : "FromPutDownTo Default Name",
    "comment" : "MISSING_COMMENT",
    "inState" : 1573418939737,
    "outState" : 1583008553235,
    "preCondition" : {
      "id" : 1583008562201,
      "name" : "1583008562201",
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