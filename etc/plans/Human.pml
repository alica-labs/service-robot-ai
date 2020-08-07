{
  "id" : 1575294066871,
  "name" : "Human",
  "comment" : "",
  "relativeDirectory" : "",
  "variables" : [ ],
  "masterPlan" : false,
  "utilityThreshold" : 0.0,
  "preCondition" : null,
  "runtimeCondition" : null,
  "entryPoints" : [ {
    "id" : 1575294081333,
    "name" : "1575294081333",
    "comment" : "",
    "successRequired" : false,
    "minCardinality" : 0,
    "maxCardinality" : 20,
    "task" : "ServiceRobotsTasks.tsk#1575291322015",
    "state" : 1575294083708,
    "plan" : 1575294066871
  } ],
  "states" : [ {
    "type" : "State",
    "id" : 1575294083708,
    "name" : "TaskGeneration",
    "comment" : "",
    "entryPoint" : 1575294081333,
    "parentPlan" : 1575294066871,
    "confAbstractPlanWrappers" : [ {
      "id" : 1596753066237,
      "name" : "1596753066237",
      "comment" : "",
      "abstractPlan" : "Behaviours/GenerateTasks.beh#1575291385685",
      "configuration" : null
    } ],
    "variableBindings" : [ ],
    "outTransitions" : [ ],
    "inTransitions" : [ ]
  } ],
  "transitions" : [ ],
  "synchronisations" : [ ]
}