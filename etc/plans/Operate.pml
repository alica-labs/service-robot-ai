{
  "id" : 1575655614484,
  "name" : "Operate",
  "comment" : "",
  "relativeDirectory" : "",
  "variables" : [ ],
  "masterPlan" : false,
  "utilityThreshold" : 0.0,
  "preCondition" : null,
  "runtimeCondition" : null,
  "entryPoints" : [ {
    "id" : 1575655644510,
    "name" : "1575655644510",
    "comment" : "",
    "successRequired" : false,
    "minCardinality" : 0,
    "maxCardinality" : 20,
    "task" : "ServiceRobotsTasks.tsk#1555601344076",
    "state" : 1575655659597,
    "plan" : 1575655614484
  }, {
    "id" : 1575655650043,
    "name" : "1575655650043",
    "comment" : "",
    "successRequired" : false,
    "minCardinality" : 0,
    "maxCardinality" : 20,
    "task" : "ServiceRobotsTasks.tsk#1575291322015",
    "state" : 1575655660934,
    "plan" : 1575655614484
  } ],
  "states" : [ {
    "type" : "State",
    "id" : 1575655659597,
    "name" : "Serve",
    "comment" : "",
    "entryPoint" : 1575655644510,
    "parentPlan" : 1575655614484,
    "abstractPlans" : [ "Serve.pml#1568825275605" ],
    "variableBindings" : [ ],
    "outTransitions" : [ ],
    "inTransitions" : [ ]
  }, {
    "type" : "State",
    "id" : 1575655660934,
    "name" : "SimHuman",
    "comment" : "",
    "entryPoint" : 1575655650043,
    "parentPlan" : 1575655614484,
    "abstractPlans" : [ "Human.pml#1575294066871" ],
    "variableBindings" : [ ],
    "outTransitions" : [ ],
    "inTransitions" : [ ]
  } ],
  "transitions" : [ ],
  "synchronisations" : [ ]
}