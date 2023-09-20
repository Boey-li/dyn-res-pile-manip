class by_Rope : public Scene
{
public:

	by_Rope(const char* name) : Scene(name) {}

	char* make_path(char* full_path, std::string path) {
		strcpy(full_path, getenv("PYFLEXROOT"));
		strcat(full_path, path.c_str());
		return full_path;
	}

	void Initialize(py::array_t<float> scene_params, int thread_idx = 0)
	{
		auto ptr = (float *) scene_params.request().ptr;

		float x = ptr[0];
		float y = ptr[1];
		float z = ptr[2];
		float length = ptr[3];
		float stiffness = ptr[4];
		float draw_mesh = ptr[5];

		float radius = 0.055f;

		int group = 0;

		char path[100];
		make_path(path, "/data/sphere.ply");
		// make_path(path, "/data/rope.ply"); # can not work
		// make_path(path, "/data/apple.obj");
		// make_path(path, "/data/pear.obj");
		// make_path(path, "/data/coffee_bean.ply");

		Vec3 velocity = RandomUnitVector()*1.0f;
		float size = 1;

		CreateParticleShape(
		        GetFilePathByPlatform(path).c_str(),
				Vec3(0., 1., 0.),
				size, 0.0f, radius, 0.0f, 0.15f, true, 1.0f, NvFlexMakePhase(0, 0), true, 0.0f);

		const int attachIndex = g_buffers->positions.size()-1;
		int start = g_buffers->positions.size();
		printf("attachIndex: %d\n", attachIndex); //0 -> -1
		printf("start: %d\n", start); //1 -> 0

		// Rope r;
		// Vec3 pos = Vec3(x, y, z);

		// // void CreateRope(Rope& rope, Vec3 start, Vec3 dir, float stiffness, int segments, float length, int phase, float spiralAngle=0.0f, float invmass=1.0f, float give=0.075f)
		// CreateRope(r, pos, Vec3(0.0f, 1.0f, 0.0f), stiffness, int(length/radius*1.1f), length, 
		// 		NvFlexMakePhase(group++, 0), 0.0f, 0.5f, 0.0f);

		// g_ropes.push_back(r);

		// const int attachIndex = g_buffers->positions.size()-1;
		// int start = g_buffers->positions.size();
		// printf("attachIndex: %d\n", attachIndex);
		// printf("start: %d\n", start);

		// CreateSpring(attachIndex, start, 1.0f);

		g_params.radius = radius;
		g_params.fluidRestDistance = radius;
		g_params.numIterations = 4;
		g_params.viscosity = 1.0f;
		g_params.dynamicFriction = 0.05f;
		g_params.staticFriction = 0.0f;
		g_params.particleCollisionMargin = 0.0f;
		g_params.collisionDistance = g_params.fluidRestDistance*0.5f;
		g_params.vorticityConfinement = 120.0f;
		g_params.cohesion = 0.0025f;
		g_params.drag = 0.06f;
		g_params.lift = 0.f;
		g_params.solidPressure = 0.0f;
		g_params.smoothing = 1.0f;
		g_params.relaxationFactor = 1.0f;

		// g_windStrength = 0.0f;
		// g_windFrequency = 0.0f;

		g_numSubsteps = 2;

		// draw options
		// if (!draw_mesh) {
        //     g_drawPoints = true;
        //     // g_drawMesh = false;
        //     g_drawRopes = false;
        //     g_drawSprings = true;
        // }
        // else {
		//     // g_drawPoints = false;
		//     // g_drawMesh = true;
		//     g_drawRopes = true;
		//     // g_drawSprings = false;
		// }

		g_drawRopes = true;
		g_drawMesh = true;
		g_drawPoints = false;
		g_drawSprings = false;

		g_ropeScale = 1.0f;
		g_warmup = false;
	}
	
};
