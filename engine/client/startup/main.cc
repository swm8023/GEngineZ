
#include <stdio.h>

#include <core/common/common.h>
#include <core/sched/sched.h>

#include <chrono>
#include <memory>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

using namespace z;

int main(int argc, char* argv[]) {
	sched::ThreadWorker worker(8);
	worker.Run();

	sched::ComplexScheduler sc(&worker);

	auto now = std::chrono::system_clock::now();
	auto sleep_nms = [&now](int n) {
		std::this_thread::sleep_for(std::chrono::milliseconds(n));

		auto new_now = std::chrono::system_clock::now();

		Log<LDEBUG>(std::this_thread::get_id(),
			"task sleep ", std::chrono::duration_cast<std::chrono::milliseconds>(new_now - now).count(), n / 1000.0);
	};

	sc.PostPar(std::bind(sleep_nms, 1000));
	sc.PostPar(std::bind(sleep_nms, 3000));
	sc.PostPar(std::bind(sleep_nms, 5000));
	sc.PostPar(std::bind(sleep_nms, 1000));

	sc.PostPar(std::bind(sleep_nms, 2000));
	sc.PostPar(std::bind(sleep_nms, 1000));
	sc.PostPar(std::bind(sleep_nms, 4000));

	sc.PostStrand(std::bind(sleep_nms, 1001));
	sc.PostStrand(std::bind(sleep_nms, 3001));
	sc.PostStrand(std::bind(sleep_nms, 5001));
	sc.PostStrand(std::bind(sleep_nms, 1001));

	worker.Stop();
	return 0;
}





/*
class WinApp {

	frame_interval
	FrameSched->Loop();
 

	view->tick();
	view->render_view->tick();

	class SceneView : View {
		class Scene {};
		class SceneRenderView {};

	};
};

*/
  
