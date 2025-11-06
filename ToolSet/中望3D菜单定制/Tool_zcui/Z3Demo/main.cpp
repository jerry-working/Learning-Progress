#include "pch.h"
#include "main.h"
#include <stdio.h>
#include <fstream>

int Z3DemoInit()
{
	cvxCmdFunc("FileCreateTemplate", FileCreateTemplate, VX_CODE_GENERAL);
	cvxCmdFunc("FileCreate", FileCreate, VX_CODE_GENERAL);
	cvxCmdFunc("FuncB", FuncB, VX_CODE_GENERAL);
	cvxCmdFunc("FileHist", FileHist, VX_CODE_GENERAL);
	
	rectorInit();

	// 生成dll，加载后即可使用zcui
	zwString512 path{};
	//ZwPathSeparatorCheck("D:\\YLRJ\\project\\PLM\\PLM_zcui\\Resource", sizeof(zwString512), path);//修改文件路径
	ZwPathSeparatorCheck("D:\\Users\\localuser\\Desktop\\整理\\工具\\zw3d界面\\凯锐PLM2\\Resource", sizeof(zwString512), path);
	ZwResourcePathAdd(path);
	ZwProfileModuleNameRegister("凯锐PLM2");//修改文件名称

	return 0;
}

int Z3DemoExit()
{
	cvxCmdFuncUnload("FileCreateTemplate");
	cvxCmdFuncUnload("FileCreate");
	cvxCmdFuncUnload("FuncB");
	cvxCmdFuncUnload("FileHist");

	rectorUnload();

	return 0;
}








int changeFile(const char* fileName1, const char* fileName2)
{
	std::ifstream in(fileName2, std::ios::binary);
	std::ofstream out("D:\\programming\\test\\Z3Demo\\x64\\Debug\\demo.Z3PRT", std::ios::binary);

	if (!in || !out) 
		return -1;
	
	out << in.rdbuf();
	
	out.close();
	in.close();

	WriteMessage("%s保存文件到%s完成", fileName2, "D:\\programming\\test\\Z3Demo\\x64\\Debug\\demo.Z3PRT");
	return 0;
}

void rectorInit()
{
	szwDocumentReactorData documentReactorData{};
	const char* uniqueName = "保存文件完成";
	strcpy_s(documentReactorData.uniqueName, uniqueName);
	documentReactorData.callbackFunction = changeFile;
	if (evxErrors::ZW_API_NO_ERROR == ZF_CALL(ZwDocumentReactorSet(ezwDocumentReactorType::ZW_DOCUMENT_SAVED, documentReactorData)))
		 WriteMessage("已添加“%s”反应器", uniqueName);
}

void rectorUnload()
{
	szwDocumentReactorData documentReactorData{};
	const char* uniqueName = "保存文件完成";
	strcpy_s(documentReactorData.uniqueName, uniqueName);
	documentReactorData.callbackFunction = NULL;
	if (evxErrors::ZW_API_NO_ERROR == ZF_CALL(ZwDocumentReactorSet(ezwDocumentReactorType::ZW_DOCUMENT_SAVED, documentReactorData)))
		WriteMessage("已注销“%s”反应器", uniqueName);
}


int FileHist()
{
	//char FileName[] = { "APITestFileSingle" };
	//char DestRoot[] = { "APICopyPart" };
	char DestFile[] = { "APICopyFile.Z3" };

	int nRet = 0;//cvxFileNameCheck(DestFile, sizeof(DestFile));
	cvxPathSet("C:\\Users\\localuser\\AppData\\Roaming\\ZWSOFT\\ZW3D\\ZW3D 2026");
	nRet = cvxFileOpen(DestFile);

	int nCount = 0;
	int* nHist = new int;

	nRet = cvxPartHistList(0, &nCount, &nHist);


	return nRet;
}


int FileCreateTemplate()
{
	char FileName[] = { "APITestFileSingle" };
	char DestRoot[] = { "APICopyPart" };
	char DestFile[] = { "APICopyFile.Z3" };

	int nRet = 0;//cvxFileNameCheck(DestFile, sizeof(DestFile));
	cvxPathSet("C:\\Users\\localuser\\AppData\\Roaming\\ZWSOFT\\ZW3D\\ZW3D 2026");
	nRet = cvxFileOpen(DestFile);
	nRet = cvxFileNewSingle(FileName, VX_FILE_PART, VX_NORMAL_PART, DestRoot, NULL);
	nRet = cvxFileSaveAs(FileName);
	cvxFileClose();
	cvxFileClose();
	return nRet;
}


/* Create a file and add a part and a sheet to it. */
/* Return 1 if function fails, else 0. */
int FileCreate(void)
{
	int iRet = 0;
	char FileName[] = { "APITestFile.Z3" };
	char PartName[] = { "APITestPart" };
	char DrawName[] = { "APITestDwg" };

	/* Create a new file with the specified name and make it active. */
	iRet = cvxFileNew(FileName);
	if (iRet)
	{
		cvxMsgDisp("Fail to create a new file.");
		return iRet;
	}

	/* Create and insert a part object to active file. */
	iRet = cvxRootAdd(VX_ROOT_PART, PartName, NULL);
	if (iRet)
	{
		cvxMsgDisp("Fail to add part.");
		return iRet;
	}

	/* Exit the active part. */
	cvxPartExit();

	/* Create and insert a sheet object to active file. */
	iRet = cvxRootAdd(VX_ROOT_SHEET, DrawName, NULL);
	if (iRet)
	{
		cvxMsgDisp("Fail to add drawing.");
		return iRet;
	}

	/* Exit the active sheet. */
	cvxPartExit();

	/* Save file. */
	iRet = cvxFileSaveAs(FileName);
	if (iRet)
	{
		cvxMsgDisp("Fail to save.");
		return iRet;
	}

	/* Close active file. */
	cvxFileClose();
	return iRet;
}

/* Copy the active file. */
/* Return 1 if function fails, else 0. */
int FuncB(void)
{
	int iRet = 0;
	char SourceFile[] = { "APISourceFile.Z3" };
	char SourceRoot[] = { "APISourcePart" };
	char DestFile[] = { "APICopyFile.Z3" };
	char DestRoot[] = { "APICopyPart" };
	svxBoxData Box;
	int idShape = -1;

	/* initialize memory */
	cvxMemZero((void*)&Box, sizeof(Box));

	Box.Center.x = 0.0;
	Box.Center.y = 0.0;
	Box.Center.z = 0.0;
	Box.X = 10.0;
	Box.Y = 10.0;
	Box.Z = 10.0;
	Box.Combine = VX_BOOL_ADD;
	Box.idPlane = 0;

	/* Create a new file with the specified name and make it active. */
	iRet = cvxFileNew(SourceFile);
	if (iRet)
	{
		cvxMsgDisp("Fail to create source file.");
		return iRet;
	}

	/* Create and insert a part object in the source file. */
	iRet = cvxRootAdd(VX_ROOT_PART, SourceRoot, NULL);
	if (iRet)
	{
		cvxMsgDisp("Fail to add part.");
		return iRet;
	}

	/* Creates a box shape and adds it to the source file. */
	iRet = cvxPartBox(&Box, &idShape);
	if (iRet)
	{
		cvxMsgDisp("Fail to create box.");
		return iRet;
	}

	/* Exit the active part. */
	cvxPartExit();

	/* Save file. */
	iRet = cvxFileSaveAs(SourceFile);
	if (iRet)
	{
		cvxMsgDisp("Fail to save.");
		return iRet;
	}

	cvxFileClose();

	/* Create another file. */
	iRet = cvxFileNew(DestFile);
	if (iRet)
	{
		cvxMsgDisp("Fail to create the destination file.");
		return iRet;
	}

	/* Copy the part to newly created file. */
	iRet = cvxRootCopy(SourceFile, SourceRoot, DestFile, DestRoot, 0, 1);
	if (iRet)
	{
		cvxMsgDisp("Fail to copy the root object.");
		return iRet;
	}

	/* Exit the active part. */
	cvxPartExit();

	/* Save file. */
	iRet = cvxFileSaveAs(DestFile);
	if (iRet)
	{
		cvxMsgDisp("Fail to save.");
		return iRet;
	}

	return iRet;
}
