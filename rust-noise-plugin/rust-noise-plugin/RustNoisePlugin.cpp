#include "RustNoisePlugin.h"

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "JungWoo", "1.0");
	status = plugin.registerCommand("rustNoise", RustNoisePlugin::creator, RustNoisePlugin::createSyntax);
	if (!status)
		status.perror("Failed register rust noise command");

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);
	status = plugin.deregisterCommand("rustNoise");
	if (!status)
		status.perror("Failed deregister rust noise command");

	return status;
}

RustNoisePlugin::RustNoisePlugin()
{
}

RustNoisePlugin::~RustNoisePlugin()
{
}

void* RustNoisePlugin::creator()
{
	return new RustNoisePlugin();
}

MSyntax RustNoisePlugin::createSyntax()
{
	MSyntax syntax;
	syntax.addFlag("-n", "-name", MSyntax::kString);
	syntax.addFlag("-d", "-displacement", MSyntax::kDouble);

	syntax.enableQuery(true);
	syntax.enableEdit(false);

	return syntax;
}

MStatus RustNoisePlugin::doIt(const MArgList& args)
{
	MStatus status;
	MArgParser argData(syntax(), args, &status);

	unsigned int objectLen = argData.numberOfFlagUses("name");
	if (objectLen != 0)
	{
		double displacementNum = 0.;
		MArgList objectList;

		argData.getFlagArgument("displacement", 0, displacementNum);
		for (int i = 0; i < objectLen; i++)
		{
			argData.getFlagArgumentList("name", i, objectList);
		}

		for (int i = 0; i < objectList.length(); i++)
		{
			MString objectName = objectList.asString(i);
			MStatus innerStatus = rustNoise(objectName, displacementNum);
			if (innerStatus)
			{
				MGlobal::displayInfo("Successfuly generate " + objectName + " noise surface!");
			}
		}

		status = MS::kSuccess;
	}
	else
	{
		MGlobal::displayError("Need to have at least one name");
		status = MS::kFailure;
	}

	return status;
}

MStatus RustNoisePlugin::rustNoise(MString objectName, double displacementNum)
{
	srand((unsigned)time(nullptr));

	MStatus status;

	MSelectionList selection;
	MDagPath dagPath;
	MItSelectionList iter(selection);
	selection.add(objectName);
	selection.getDagPath(0, dagPath);
	MItGeometry geoIter(dagPath);
	MPointArray pArray;
	status = geoIter.allPositions(pArray, MSpace::kWorld);

	if (status != MS::kSuccess)
	{
		MGlobal::displayError("Failed get " + objectName + " positions");
		return status;
	}

	for (int i = 0; i < pArray.length(); i++)
	{
		struct NoiseVector noise = new_rust_noise_vector();
		gen_noise(&noise, displacementNum);
		pArray[i].x += noise.x;
		pArray[i].y += noise.y;
		pArray[i].z += noise.z;
	}

	status = geoIter.setAllPositions(pArray, MSpace::kWorld);
	if (status != MS::kSuccess)
	{
		MGlobal::displayError("Failed set " + objectName + " positions");
		return status;
	}

	MFnMesh meshFn(dagPath);
	status = meshFn.updateSurface();
	if (status != MS::kSuccess)
	{
		MGlobal::displayError("Failed update " + objectName + " surface");
		return status;
	}

	return status;
}
