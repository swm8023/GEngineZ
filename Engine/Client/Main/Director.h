#pragma once
#include <Client/Scene/Scene.h>
#include <Core/CoreHeader.h>
#include <Render/Renderer.h>
#include <Client/Scene/Camera.h>


namespace z {

class CameraController;
class Director {

public:
	Director();
	~Director();
	
	void Update();
	
	void FrameTick();
	void BeginFrame();
	void EndFrame();

	void SetFrameLimit(int limit) {
		mFrameInterval = int(1000.0f / limit);
	}

	void SetCameraController(CameraController *);

	CameraController* GetCameraController() {
		return mCameraController;
	}

	void LoadScene(const std::string& scnFile);

	Scene* GetCurScene() {
		return mCurScene;
	}

	Renderer* GetRenderer() {
		return mRenderer;
	}

	void OnResize(uint32_t width, uint32_t height) {
		if (mRenderer) {
			mRenderer->Resize(width, height);
		}
		if (mCurScene) {
			mCurScene->GetCamera()->SetAspect(float(width) / height);
		}
	}

	float GetCurFps() {
		return mCurFps;
	}

	uint32_t GetDrawCalls() {
		return mRHIStats.DrawCalls;
	}

	uint32_t GetDrawFaces() {
		return mRHIStats.Faces;
	}

	float GetFrameTime() {
		return mFrameTime;
	}

private:
	uint64_t mFrameInterval{ 0 };
	float mFrameTime{ 0 };
	
	float mCurFps{ 0 };
	int mFramesForStatFps{ 0 };
	uint64_t mFpsStatTime{ 0 };

	RefCountPtr<Scene> mCurScene;
	RefCountPtr<Renderer> mRenderer;
	RefCountPtr<CameraController> mCameraController;

	RHIStats mRHIStats;
	
};

extern Director* GDirector;


}

