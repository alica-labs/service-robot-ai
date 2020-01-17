% Module Property Einhaltung
% InputQuery => Erweitert Sensorinformationen mit Timestamp + External zum entfernen
% 	Erlaubt Verwendung von input (is) in Regeln ohne Anpassung des query Prädikats in den situationalknowledge Regeln
%	Kein Anpassung der Stelligkeit notwendig
%	Verwendung von eigenem External erlaubt das Löschen des Inputs entweder nach Ende der Lifetime oder auf Wunsch des Nutzers
% 
% Neue Query für situationalknowledge
%	Erlaubt Weiterverwendung der bestehenden PrgramSection
%	Einsatz von 2. External zum Rausnehmen des Wissens
%	Paar von Extenals zum Umschalten des Widerspruchs
%	Wissen nur durch Add hinzugefügt => keine Inkonsistenz in der Wissensbasis
%	Keine Inkonsistenz durch Hinzufügen aus cs
%	Unterstützung durch bisherige Queries nicht möglich

is(plant1, cs_evergreen, 1234).
is(plant1, cs_decidouos, 1234).

#external ext.

cs_Antonym(cs_evergreen, cs_decidouos, 10).
cs_Antonym(cs_decidouos, cs_evergreen, 10).

#external -propertyOf(plant1, cs_evergreen).
#external -propertyOf(plant1, cs_decidouos).

propertyOf(plant1, cs_evergreen, W) :- not -propertyOf(plant1, cs_evergreen), is(plant1, cs_evergreen, _), cs_Antonym(cs_evergreen, cs_decidouos, W), -propertyOf(plant1, cs_decidouos), ext.
propertyOf(plant1, cs_decidouos, W) :- not -propertyOf(plant1, cs_decidouos), is(plant1, cs_decidouos, _), cs_Antonym(cs_decidouos, cs_evergreen, W), -propertyOf(plant1, cs_evergreen), ext.

%V2

is(plant1, cs_evergreen, 1).
is(plant1, cs_decidouos, 2).

cs_Antonym(cs_evergreen, cs_decidouos, 10).
cs_Antonym(cs_decidouos, cs_evergreen, 10).

#external -hasProperty(plant1, cs_evergreen).
#external -hasProperty(plant1, cs_decidouos).
#external ext2.

hasProperty(plant1, cs_evergreen, W) :- not -hasProperty(plant1, cs_evergreen), is(plant1, cs_evergreen, _), cs_Antonym(cs_evergreen, cs_decidouos, W), -hasProperty(plant1, cs_decidouos), ext2.
hasProperty(plant1, cs_decidouos, W) :- not -hasProperty(plant1, cs_decidouos), is(plant1, cs_decidouos, _), cs_Antonym(cs_decidouos, cs_evergreen, W), -hasProperty(plant1, cs_evergreen), ext2.

%V2.5
is(plant1, cs_evergreen).
is(plant1, cs_decidouos).

cs_Antonym(cs_evergreen, cs_decidouos, 10).
cs_Antonym(cs_decidouos, cs_evergreen, 10).

#external -hasProperty(plant1, cs_evergreen).
#external -hasProperty(plant1, cs_decidouos).
#external ext2.

hasProperty(plant1, cs_evergreen, W) :- not -hasProperty(plant1, cs_evergreen), is(plant1, cs_evergreen, _),  -hasProperty(plant1, cs_decidouos) : cs_Antonym(cs_evergreen, cs_decidouos, W); ext2.
hasProperty(plant1, cs_decidouos, W) :- not -hasProperty(plant1, cs_decidouos), is(plant1, cs_decidouos, _), -hasProperty(plant1, cs_evergreen) :  cs_Antonym(cs_decidouos, cs_evergreen, W); ext2.

%V3
is(plant1, cs_evergreen).
is(plant1, cs_decidouos).

cs_Antonym(cs_evergreen, cs_decidouos, 10).
cs_Antonym(cs_decidouos, cs_evergreen, 10).

#external -hasProperty(plant1, cs_evergreen).
#external -hasProperty(plant1, cs_decidouos).
#external ext2.

hasProperty(plant1, cs_evergreen, W) :- not -hasProperty(plant1, cs_evergreen), is(plant1, cs_evergreen, _),  is(plant1, cs_plant), cs_Antonym(cs_evergreen, cs_decidouos, W), -hasProperty(plant1, cs_decidouos), ext2.
hasProperty(plant1, cs_decidouos, W) :- not -hasProperty(plant1, cs_decidouos), is(plant1, cs_decidouos, _), cs_Antonym(cs_decidouos, cs_evergreen, W), -hasProperty(plant1, cs_evergreen), ext2.

hasProperty(plant1, evergreen, W) :- not -hasProperty(plant1, evergreen), is(plant1, plant, _), cs_hasProperty(plant, evergreen, W), ext2.
hasProperty(plant1, decidouos, W) :- not -hasProperty(plant1, decidouos), is(plant1, plant, _), cs_hasProperty(plant, decidouos, W), ext2.

%V4
is(plant1, cs_evergreen, 10).
is(plant1, cs_decidouos, 10).
is(plant1, cs_plant, 10).

cs_Antonym(cs_evergreen, cs_decidouos, 10).
cs_Antonym(cs_decidouos, cs_evergreen, 10).

#external -hasProperty(plant1, cs_evergreen).
#external -hasProperty(plant1, cs_decidouos).
#external ext2.

hasProperty(plant1, cs_evergreen) :- not -hasProperty(plant1, cs_evergreen), is(plant1, cs_evergreen, _),  is(plant1, cs_plant, _), -hasProperty(plant1, cs_decidouos):cs_Antonym(cs_evergreen, cs_decidouos, W); ext2.
hasProperty(plant1, cs_decidouos) :- not -hasProperty(plant1, cs_decidouos), is(plant1, cs_decidouos, _), is(plant1, cs_plant, _), -hasProperty(plant1, cs_evergreen):cs_Antonym(cs_decidouos, cs_evergreen, W); ext2.

%V5
is(plant1, plant, 10) :- ext.

%fest
cs_hasProperty(plant, evergreen, 30).
cs_hasProperty(plant, decidouos, 30).
cs_Antonym(evergreen, decidouos, 10).
cs_Antonym(decidouos, evergreen, 10).

%fest
#external -hasProperty(plant1, evergreen).
#external -hasProperty(plant1, decidouos).
#external ext2.

hasProperty(plant1, evergreen, W) :- not -hasProperty(plant1, evergreen), cs_hasProperty(plant, evergreen, W),  is(plant1, plant, _), -hasProperty(plant1, decidouos):cs_Antonym(evergreen, decidouos, W); ext2.
hasProperty(plant1, decidouos, W) :- not -hasProperty(plant1, decidouos), cs_hasProperty(plant, decidouos, W), is(plant1, plant, _), -hasProperty(plant1, evergreen):cs_Antonym(decidouos, evergreen, W); ext2.


%V6?
#external is(plant1, plant, 10).

cs_hasProperty(plant, evergreen, 30).
cs_hasProperty(plant, decidouos, 30).
cs_Antonym(evergreen, decidouos, 10).
cs_Antonym(decidouos, evergreen, 10).

#external -hasProperty(plant1, evergreen).
#external -hasProperty(plant1, decidouos).

hasProperty(plant1, evergreen, W) :- not -hasProperty(plant1, evergreen), cs_hasProperty(plant, evergreen, W),  is(plant1, plant, _), -hasProperty(plant1, decidouos):cs_Antonym(evergreen, decidouos, W).
hasProperty(plant1, decidouos, W) :- not -hasProperty(plant1, decidouos), cs_hasProperty(plant, decidouos, W),  is(plant1, plant, _), -hasProperty(plant1, evergreen):cs_Antonym(decidouos, evergreen, W).
