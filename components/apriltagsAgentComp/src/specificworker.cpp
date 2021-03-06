/*
 *    Copyright (C) 2006-2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "specificworker.h"

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)	
{
	active = false;

	worldModel = AGMModel::SPtr(new AGMModel());
	worldModel->name = "worldModel";

	innerModel = new InnerModel("/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/ursusMM.xml");
	trTag = innerModel->newTransform("trTag", "static", innerModel->getTransform("rgbd_transform"));
	innerModel->getTransform("rgbd_transform")->addChild(trTag);


}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}


void SpecificWorker::updateInnerModel(AGMModel::SPtr newModel)
{
	//
	// Update InnerModel using the joints of the robot
	MotorStateMap mstateMap;
	jointmotor_proxy->getAllMotorState(mstateMap);
	for (MotorStateMap::iterator it= mstateMap.begin(); it!=mstateMap.end(); it++)
	{
		innerModel->updateJointValue(QString::fromStdString(it->first), it->second.pos);
	}
	/// Update wrist position in the robot symbol according to innermodel or the corresponding April tag
	updateWristPose();
}

void SpecificWorker::removeUnseenTags(AGMModel::SPtr newModel)
{
	//
	// Remove tags which have not been seen in two seconds
	QTime now = QTime::currentTime(); // Get current time
	for (TagModelMap::iterator i=tagMap.begin();  i!=tagMap.end(); )
	{
		if (i->second.lastTime.msecsTo(now) > 2000)
		{
			printf("Removing %d\n", i->second.id);
			tagMap.erase(i++);
		}
		else
		{
			++i;
		}
	}
}

void SpecificWorker::includeOrUpdateTags(AGMModel::SPtr newModel, bool &modelWasModified)
{
	// Include new tags which are not currently in the model and update those which are
	for (TagModelMap::iterator itMap=tagMap.begin();  itMap!=tagMap.end(); itMap++)
	{
		bool found = false;
		for (AGMModel::iterator itModel=worldModel->begin(); itModel!=worldModel->end(); itModel++)
		{
			if (itModel->symbolType == "ball")
			{
				if (itMap->second.id == str2int(itModel->attributes["id"]))
				{
					found = true;
					updateSymbolWithTag(*itModel, itMap->second);
					break;
				}
			}
		}
		if (not found)
		{
			includeObjectInModel(newModel, itMap->second);
			modelWasModified = true;
		}
	}
}


void SpecificWorker::compute( )
{
	// Create a copy of the model
	bool modelWasModified = false;
	AGMModel::SPtr newModel(new AGMModel(worldModel));

	// Update InnerModel
	updateInnerModel(newModel);
	
	// Remove unseen tags from map
	removeUnseenTags(newModel);
	

	// Get the symbol of the robot and its status (just in case)
	int32_t statusId = newModel->getIdentifierByType("status");
	int32_t robotId = newModel->getIdentifierByType("robot");
	if (robotId == -1 or statusId == -1)
	{
		printf("didn't find the robot or its status!!\n");
		return;
	}
	AGMModelSymbol::SPtr robot = newModel->getSymbol(robotId);

	
	// Include or update tags in the model
	includeOrUpdateTags(newModel, modelWasModified);

	// Remove balls which have not been seen in a while
	for (AGMModel::iterator itModel=worldModel->begin(); itModel!=worldModel->end(); itModel++)
	{
		if (itModel->symbolType == "ball")
		{
			bool found = false;
			for (TagModelMap::iterator itMap=tagMap.begin();  itMap!=tagMap.end(); itMap++)
			{
				if (itMap->second.id == str2int(itModel->attributes["id"]))
				{
					found = true;
					break;
				}
			}
			if (not found)
			{
				modelWasModified = true;
				newModel->removeSymbol(itModel->identifier);
				if (newModel->removeEdgeByIdentifiers(robotId, statusId, "happy"))
					newModel->addEdgeByIdentifiers(robotId, statusId, "bored");
				newModel->removeDanglingEdges();
			}
		}
	}

	// Send modification if needed
	if (modelWasModified)
	{
		sendModificationProposal(worldModel, newModel);
	}
}


void SpecificWorker::updateSymbolWithTag(AGMModelSymbol::SPtr symbol, const AprilTagModel &tag)
{
	innerModel->updateTransformValues("trTag", tag.tx, tag.ty, tag.tz, tag.rx+M_PIl, tag.ry, tag.rz);
	QVec T = innerModel->transform("robot", QVec::vec3(0,0,0), "trTag");
	QVec R = innerModel->getRotationMatrixTo("trTag", "robot").extractAnglesR_min();

   //threshold to update set to 20 mm and 0.1 rad
   if ( fabs(str2float((symbol->attributes["tx"])) - T(0)) > 20  ||
	    fabs(str2float((symbol->attributes["ty"])) - T(1)) > 20  ||
	    fabs(str2float((symbol->attributes["tz"])) - T(2)) > 20  ||
	    fabs(str2float((symbol->attributes["rx"])) - R(0)) > 0.1 ||
	    fabs(str2float((symbol->attributes["ry"])) - R(1)) > 0.1 ||
	    fabs(str2float((symbol->attributes["rz"])) - R(2)) > 0.1 )
	{
		symbol->attributes["tx"] = float2str(T(0));
		symbol->attributes["ty"] = float2str(T(1));
		symbol->attributes["tz"] = float2str(T(2));
		symbol->attributes["rx"] = float2str(R(0));
		symbol->attributes["ry"] = float2str(R(1));
		symbol->attributes["rz"] = float2str(R(2));
		RoboCompAGMWorldModel::Node symbolICE;
		AGMModelConverter::fromInternalToIce(symbol, symbolICE);
		agmagenttopic->update(symbolICE);
	}
}

void SpecificWorker::includeObjectInModel(AGMModel::SPtr &newModel, const AprilTagModel &tag)
{
	int32_t robotId = newModel->getIdentifierByType("robot");
	if (robotId == -1)
	{
		return;
	}
	/// object
	AGMModelSymbol::SPtr newTag = newModel->newSymbol("ball");
	newModel->addEdgeByIdentifiers(robotId, newTag->identifier, "sees");
	newTag->attributes["id"] = int2str(tag.id);
	newTag->attributes["tx"] = float2str(tag.tx);
	newTag->attributes["ty"] = float2str(tag.ty);
	newTag->attributes["tz"] = float2str(tag.tz);
	newTag->attributes["rx"] = float2str(tag.rx);
	newTag->attributes["ry"] = float2str(tag.ry);
	newTag->attributes["rz"] = float2str(tag.rz);
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	timer.start(Period);
	return true;
}

bool SpecificWorker::activateAgent(const ParameterMap& prs)
{
	bool activated = false;
	if (setParametersAndPossibleActivation(prs, activated))
	{
		if (not activated)
		{
			mutex->lock();
			active = true;
			mutex->unlock();
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool SpecificWorker::deactivateAgent()
{
	mutex->lock();
	active = false;
	mutex->unlock();
	return true;
}

StateStruct SpecificWorker::getAgentState()
{
	StateStruct s;
	if (active)
	{
		s.state = Running;
	}
	else
	{
		s.state = Stopped;
	}
	s.info = action;
	return s;
}

bool SpecificWorker::setParametersAndPossibleActivation(const ParameterMap &prs, bool &reactivated)
{
	// We didn't reactivate the component
	reactivated = false;

	// Update parameters
	params.clear();
	for (ParameterMap::const_iterator it=prs.begin(); it!=prs.end(); it++)
	{
		params[it->first] = it->second;
	}

	try
	{
		action = params["action"].value;
		std::transform(action.begin(), action.end(), action.begin(), ::tolower);

		if (action == "graspobject")
		{
			active = true;
		}
		else
		{
			active = true;
		}
	}
	catch (...)
	{
		printf("exception in setParametersAndPossibleActivation %d\n", __LINE__);
		return false;
	}

	// Check if we should reactivate the component
	if (active)
	{
		active = true;
		reactivated = true;
	}

	return true;
}

ParameterMap SpecificWorker::getAgentParameters()
{
	return params;
}

bool SpecificWorker::setAgentParameters(const ParameterMap& prs)
{
	bool activated = false;
	return setParametersAndPossibleActivation(prs, activated);
}

void SpecificWorker::killAgent()
{
	exit(-1);
}

int SpecificWorker::uptimeAgent()
{
	return 0;
}

bool SpecificWorker::reloadConfigAgent()
{
	return true;
}

void SpecificWorker::modelModified(const RoboCompAGMWorldModel::Event& modification)
{
	mutex->lock();
	AGMModelConverter::fromIceToInternal(modification.newModel, worldModel);
	mutex->unlock();	
}

void SpecificWorker::modelUpdated(const RoboCompAGMWorldModel::Node& modification)
{
	mutex->lock();
	AGMModelConverter::includeIceModificationInInternalModel(modification, worldModel);
	mutex->unlock();
}

void SpecificWorker::newAprilTag(const tagsList& tags)
{
	QTime c = QTime::currentTime();

	for (uint32_t i=0; i<tags.size(); i++)
	{
		if (tagMap.find(tags[i].id) != tagMap.end())
		{
			tagMap[tags[i].id] = AprilTagModel();
		}
		else
		{
			printf("new tag %d\n", tags[i].id);
		}

		tagMap[tags[i].id].id = tags[i].id;
		tagMap[tags[i].id].tx = tags[i].tx;
		tagMap[tags[i].id].ty = tags[i].ty;
		tagMap[tags[i].id].tz = tags[i].tz;
		tagMap[tags[i].id].rx = tags[i].rx;
		tagMap[tags[i].id].ry = tags[i].ry;
		tagMap[tags[i].id].rz = tags[i].rz;
		tagMap[tags[i].id].lastTime = c;
	}
}


void SpecificWorker::updateWristPose()
{
	// Make sure we have the robot in the model, otherwise there's nothing to do yet...
	int32_t robotId = worldModel->getIdentifierByType("robot");
	if (robotId == -1)
	{
		return;
	}
	AGMModelSymbol::SPtr robot = worldModel->getSymbol(robotId);

	// Set current T and R
	QVec T = innerModel->transform("robot", QVec::vec3(0,0,0), "arm_right_8");
	QVec R = innerModel->getRotationMatrixTo("arm_right_8", "robot").extractAnglesR_min();

	// Set back T and R
	
	QVec T_back, R_back;
	bool force = false;
	try
	{
		T_back = QVec::vec3(
		  str2float(robot->attributes["rightwrist_tx"]),
		  str2float(robot->attributes["rightwrist_ty"]),
		  str2float(robot->attributes["rightwrist_tz"]));
		R_back = QVec::vec3(
		  str2float(robot->attributes["rightwrist_rx"]),
		  str2float(robot->attributes["rightwrist_ry"]),
		  str2float(robot->attributes["rightwrist_rz"]));
	}
	catch(...)
	{
		printf("exception in updateWristPose %d\n", __LINE__);
		force = true;
	}
	#warning These thresholds should be set in the config file!!!
	#warning These thresholds should be set in the config file!!!
	#warning These thresholds should be set in the config file!!!
	#warning These thresholds should be set in the config file!!!
	if ( force or (T-T_back).norm2()>15 or (R-R_back).norm2()>0.05)
	{
		robot->attributes["rightwrist_tx"] = float2str(T(0));
		robot->attributes["rightwrist_ty"] = float2str(T(1));
		robot->attributes["rightwrist_tz"] = float2str(T(2));
		robot->attributes["rightwrist_rx"] = float2str(R(0));
		robot->attributes["rightwrist_ry"] = float2str(R(1));
		robot->attributes["rightwrist_rz"] = float2str(R(2));
		RoboCompAGMWorldModel::Node nodeDst;
		AGMModelConverter::fromInternalToIce(robot, nodeDst);
		agmagenttopic->update(nodeDst);
	}
}

void SpecificWorker::sendModificationProposal(AGMModel::SPtr &worldModel, AGMModel::SPtr &newModel)
{
	try
	{		
		//AGMModelPrinter::printWorld(newModel);
		AGMMisc::publishModification(newModel, agmagenttopic, worldModel, "april");
	}
	catch(...)
	{
		exit(1);
	}
}

