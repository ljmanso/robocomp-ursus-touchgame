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
#include <agm.h>
#include <boost/concept_check.hpp>


/**
* \brief Default constructor
*/

SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
	setlocale (LC_ALL, "C");

	worldModel = AGMModel::SPtr(new AGMModel());
	worldModel->name = "worldModel";
	active = false;

	innerModel = new InnerModel("/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/ursusMM.xml");
	currenState =STOP;

	exec=false;

}

void SpecificWorker::ballFound()
{
	static float a = 0;

	float tx =    0. + 500.*cos(a);
	float ty = 1000. + 200.*sin(a);
	float tz =  800.;
	QVec poseTr = innerModel->transform("world", QVec::vec3(tx,ty,tz), "robot");
	printf("gooooooo T=(%.2f, %.2f, %.2f)\n", poseTr(0), poseTr(1), poseTr(2));

	sendRightHandPose(QVec::vec3(400, 400, 400), QVec::vec3(0,0,0), QVec::vec3(1,1,1), QVec::vec3(0,0,0));		
	saccadic3D(poseTr,QVec::vec3(0,    -1,   0));
	
	usleep(300000);
	a += 0.18;
}

void SpecificWorker::ballCentered()
{
	int32_t ball = atoi(params["b"].value.c_str());
	int32_t robot = atoi(params["r"].value.c_str());
	try
	{
		const float tx = str2float(worldModel->getSymbol(ball)->getAttribute("tx"));
		const float ty = str2float(worldModel->getSymbol(ball)->getAttribute("ty"));
		const float tz = str2float(worldModel->getSymbol(ball)->getAttribute("tz"));
		QVec poseTr = innerModel->transform("world", QVec::vec3(tx,ty,tz), "robot");
		printf("gooooooo T=(%.2f, %.2f, %.2f)\n", poseTr(0), poseTr(1), poseTr(2));				
		saccadic3D(poseTr,QVec::vec3(0,-1,0));
		
		///meter en el modelo en el arco fixates
		worldModel->addEdgeByIdentifiers(robot,ball,"fixates");
		bool modify =true;
		if (modify)
		{
			printf("MODIFIED!\n");
			try
			{		
				RoboCompAGMWorldModel::Event e;
				AGMModel::SPtr newModel(new AGMModel(worldModel));
				e.why = RoboCompAGMWorldModel::BehaviorBasedModification;
				AGMModelConverter::fromInternalToIce(worldModel, e.backModel);
				AGMModelConverter::fromInternalToIce(newModel, e.newModel);
				// 	AGMModelPrinter::printWorld(newModel);
				agmagenttopic->modificationProposal(e);
			}
			catch(...)
			{
				exit(1);
			}
		}
		
	}
	catch(AGMModelException &e)
	{
		printf("I don't know object %d\n", ball);
	}
}


void SpecificWorker::ballTouched()
{
	int32_t ball = atoi(params["b"].value.c_str());
	int32_t robot = atoi(params["r"].value.c_str());
	int32_t status = atoi(params["s"].value.c_str());

	try
	{
		const float tx = str2float(worldModel->getSymbol(ball)->getAttribute("tx"));
		const float ty = str2float(worldModel->getSymbol(ball)->getAttribute("ty"));
		const float tz = str2float(worldModel->getSymbol(ball)->getAttribute("tz"));
		const float rwtx = str2float(worldModel->getSymbol(robot)->getAttribute("rightwrist_tx"));
		const float rwty = str2float(worldModel->getSymbol(robot)->getAttribute("rightwrist_ty"));
		const float rwtz = str2float(worldModel->getSymbol(robot)->getAttribute("rightwrist_tz"));

		QVec offset = QVec::vec3(0., -60., 0.);
		QVec poseTr = innerModel->transform("world", QVec::vec3(tx,ty,tz)+offset, "robot");
		QVec poseWrist = QVec::vec3(rwtx, rwty, rwtz)+offset;

		printf("gooooooo T=(%.2f, %.2f, %.2f)  \n", poseTr(0), poseTr(1), poseTr(2));
		sendRightHandPose(poseTr, QVec::vec3(0,0,0), QVec::vec3(1,1,1), QVec::vec3(0,0,0));		
		
		sleep(10);
		//borrar sees y meter touches.
		worldModel->removeEdgeByIdentifiers(robot,ball,"sees");
		worldModel->addEdgeByIdentifiers(robot,ball,"touches");
		
		//borrar bored por happy
		worldModel->removeEdgeByIdentifiers(robot,status,"bored");
		worldModel->addEdgeByIdentifiers(robot,status,"happy");
		
		bool modify =true;
		if (modify)
		{
			printf("MODIFIED!\n");
			try
			{		
				RoboCompAGMWorldModel::Event e;
				AGMModel::SPtr newModel(new AGMModel(worldModel));
				e.why = RoboCompAGMWorldModel::BehaviorBasedModification;
				AGMModelConverter::fromInternalToIce(worldModel, e.backModel);
				AGMModelConverter::fromInternalToIce(newModel, e.newModel);
				printf("<<%d\n", newModel->numberOfSymbols());
				// 	AGMModelPrinter::printWorld(newModel);
				agmagenttopic->modificationProposal(e);
				printf(">>\n");
			}
			catch(...)
			{
				exit(1);
			}
		}
		
	}
	catch(AGMModelException &e)
	{
		printf("I don't know object %d\n", ball);
	}
}

void SpecificWorker::resetGame()
{
	sleep(3);
	
	
	int32_t robot = worldModel->getIdentifierByType("robot");
	int32_t status = worldModel->getIdentifierByType("status");

	worldModel->removeEdgeByIdentifiers(robot, status, "happy");
	worldModel->addEdgeByIdentifiers(robot, status, "bored");

	for (AGMModel::iterator itModel=worldModel->begin(); itModel!=worldModel->end(); itModel++)
	{
		if (itModel->symbolType == "ball")
		{
			worldModel->removeSymbol(itModel->identifier);
		}
	}

	try
	{		
		RoboCompAGMWorldModel::Event e;
		AGMModel::SPtr newModel(new AGMModel(worldModel));
		e.why = RoboCompAGMWorldModel::BehaviorBasedModification;
		AGMModelConverter::fromInternalToIce(worldModel, e.backModel);
		AGMModelConverter::fromInternalToIce(worldModel, e.newModel);
		//AGMModelPrinter::printWorld(worldModel);
		agmagenttopic->modificationProposal(e);
	}
	catch(...)
	{
		exit(1);
	}
}

///slot
void SpecificWorker::compute( )
{
	usleep(500000);

	printf("action: %s\n", action.c_str());
	if (action == "ballfound")
	{
		ballFound();
		
	}
	else if (action == "ballcentered")
	{
		ballCentered();
		
	}
	else if (action == "balltouched" )
	{
		ballTouched();
	}
	else if (action == "none" )
	{
		resetGame();
	}
	else
	{
		printf("ignoring this action (%s)...\n", action.c_str());
	}

}

void SpecificWorker::slotStop()
{
	currenState=STOP;
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


		if (action == "findobjectvisually")
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

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}


void SpecificWorker::sendRightHandPose(QVec t, QVec r, QVec wt, QVec wr)
{
	sendRightHandPose(t, r, wt(0), wt(1), wt(2), wr(0), wr(1), wr(2));
}

void SpecificWorker::sendRightHandPose(QVec t, QVec r, float wtx, float wty, float wtz, float wrx, float wry, float wrz)
{
	sendRightHandPose(t(0), t(1), t(2), r(0), r(1), r(2), wtx, wty, wtz, wrx, wry, wrz);
}

void SpecificWorker::sendRightHandPose(float tx, float ty, float tz, float rx, float ry, float rz, float wtx, float wty, float wtz, float wrx, float wry, float wrz)
{
	RoboCompBodyInverseKinematics::Pose6D target;
	target.x = tx;
	target.y = ty;
	target.z = tz;
	target.rx = rx;
	target.ry = ry;
	target.rz = rz;
	RoboCompBodyInverseKinematics::WeightVector weights;
	weights.x = wtx;
	weights.y = wty;
	weights.z = wtz;
	weights.rx = wrx;
	weights.ry = wry;
	weights.rz = wrz;
	try
	{
		bodyinversekinematics_proxy->setTargetPose6D("RIGHTARM", target, weights, 100);
	}
	catch(...)
	{
		printf("IK connection error\n");
	}
}


void SpecificWorker::saccadic3D(QVec point, QVec axis)
{
	saccadic3D(point(0), point(1), point(2), axis(0), axis(1), axis(2));
}

void SpecificWorker::saccadic3D(float tx, float ty, float tz, float axx, float axy, float axz)
{
	RoboCompBodyInverseKinematics::Pose6D targetSight;
	targetSight.x = tx;
	targetSight.y = ty;
	targetSight.z = tz;
	RoboCompBodyInverseKinematics::Axis axSight;
	axSight.x = axx;
	axSight.y = axy;
	axSight.z = axz;
	bool axisConstraint = false;
	float axisAngleConstraint = 0;
	try
	{
		bodyinversekinematics_proxy->pointAxisTowardsTarget("HEAD", targetSight, axSight, axisConstraint, axisAngleConstraint);
	}
	catch(...)
	{
		printf("IK connection error\n");
	}
}




