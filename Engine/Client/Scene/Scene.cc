#include "Scene.h"
#include <Render/SceneCollection.h>
#include <Render/RenderItem.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIResource.h>

#include <Client/Main/App.h>
#include <Client/Scene/Camera.h>
#include <Client/Entity/IEntity.h>
#include <Util/Mesh/ZMeshLoader.h>
#include <Util/Image/Image.h>
#include <Util/Mesh/MeshGenerator.h>
#include <Util/Luaconf/Luaconf.h>
#include <Client/Component/PrimitiveComp.h>
#include <Client/Component/EnvComp.h>

namespace z {


Scene::Scene() {
	mIsEditor = true;
}


bool Scene::Load(const std::string file) {
	mCamera = new Camera(math::Vector3F{ -8, 3, 3 }, math::Vector3F(0, 0, 0));

	if (mIsEditor) {
		// grid, 10m*10m
		RenderMesh *mesh = MeshGenerator::CreateGrid(380, 380, 19, 19);
///		RenderMesh *mesh = MeshGenerator::CreateGrid(1580, 1580, 79, 79);
		RenderItem* item = new RenderItem();
		item->RenderSet = RENDER_SET_EDITOR;
		item->Material = MaterialManager::GetMaterialInstance("EditorAxis");
        item->Material->SetParameter("Color", math::Vector3F(0.1f, 0.1f, 0.1f).value, 3);
		item->Material->SetFillMode(RS_FILL_WIREFRAME);
		item->Mesh = mesh;
		item->SetMeshIndexGroup(0);
		item->WorldMatrix = math::Matrix4F::Identity;
		mEditorItems.push_back(item);

		// xyz axis
		for (int i = 0; i < 3; i++) {
			RenderMesh*mesh = MeshGenerator::CreateCylinder(0.1f, 0, 20, 10, 10);
			RenderItem *item = new RenderItem();
			item->RenderSet = RENDER_SET_EDITOR;
			item->Material = MaterialManager::GetMaterialInstance("EditorAxis");
			item->Material->SetParameter("Color", math::Vector3F(i==0, i==1, i==2).value, 3);
			//item->material->SetFillMode(RS_FILL_WIREFRAME);
			item->Mesh = mesh;
			item->SetMeshIndexGroup(0);
			item->WorldMatrix = math::Matrix4F::Identity;

			math::Matrix4F rotation = math::MatrixRotationAxis(math::Vector3F(i==2, i==1, i==0), math::ToRadian(90.f * (3 - i)));
			item->WorldMatrix = item->WorldMatrix * rotation;
			
			mEditorItems.push_back(item);
		}


	}

	if (!LoadFromFile(file)) {
		Log<LERROR>("Load scene failed!!!");
		return false;
	}

	Log<LINFO>("Load scene success...");
	return true;
}


bool Scene::LoadFromFile(const std::string file) {
	namespace lc = luaconf;
	lc::Value sceneData;
	if (!lc::ParseFile(file, sceneData)) {
		return false;
	}

	lc::Value envData = sceneData["env"];

	lc::Value skyData = envData["sky"];

	// scene env
	EnvComp *env = new EnvComp();
	env->Attach(this);

	// create sky
	std::string skyShader = skyData["shader"].Get<lc::string_t>();
	std::string skyTexture = GApp->GetContentPath() / skyData["texture"].Get<lc::string_t>();

	env->SetSky(skyShader, skyTexture);

	// create models
	int modelNum = sceneData["models"].Size();
	for (int modelIdx = 0; modelIdx < modelNum; modelIdx++) {
		lc::Value modelData = sceneData["models"][modelIdx];
		RefCountPtr<IEntity> ent = new IEntity();
		std::vector<float> entPos, entScale, entRotator;
		if (!modelData["position"].GetValSafety(entPos)) {
			entPos = { 0, 0, 0 };
		}
		if (!modelData["scale"].GetValSafety(entScale)) {
			entScale = { 1, 1, 1 };
		}
		if (!modelData["rotator"].GetValSafety(entRotator)) {
			entRotator = { 0, 0, 0 };
		}
		ent->SetLocalPostion({ entPos[0], entPos[1], entPos[2] });
		ent->SetLocalScale({ entScale[0], entScale[1], entScale[2] });
		ent->SetLocalRotator({ math::ToRadian(entRotator[0]),  math::ToRadian(entRotator[1]), entRotator[2] });

		RefCountPtr<PrimitiveComp> p = new PrimitiveComp();
		if (p->LoadFromFile(GApp->GetContentPath() / modelData["model"].Get<lc::string_t>())) {
			p->Attach(ent);
			mEntities.push_back(ent);
		}
	}
	return true;
}



void Scene::CollectRender(SceneCollection* collection) {
	// camera info
	collection->SetCamera(
		mCamera->GetCam()->GetPosition(),
		mCamera->GetCam()->GetViewMatrix(),
		mCamera->GetCam()->GetProjMatrix()
	);

	EnvComp *comp = GetComponent<EnvComp>();
	comp->CollectRender(collection);

	// editor items
	for (RenderItem* item : mEditorItems) {
		collection->PushRenderItem(item);
	}

	// primitives
	for (IEntity* ent : mEntities) {
		std::vector<PrimitiveComp*> prims = ent->GetComponents<PrimitiveComp>();
		for (auto prim : prims) {
			prim->CollectRender(collection);
		}
	}

}

IEntity* Scene::Pick(const math::Vector3F& rayStart, const math::Vector3F& rayDir) {
    for (IEntity* ent : mEntities) {
        if (!ent->IsPickable())
            continue;

        // convert to local ray



        std::vector<PrimitiveComp*> prims = ent->GetComponents<PrimitiveComp>();
        for (auto prim : prims) {
            //if (prim->IsIntersectRay)

        }
    }
}


}