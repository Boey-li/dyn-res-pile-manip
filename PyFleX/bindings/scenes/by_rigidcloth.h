class by_RigidCloth: public Scene
{
public:

	by_RigidCloth(const char* name) : Scene(name) {}

	char* make_path(char* full_path, std::string path) {
		strcpy(full_path, getenv("PYFLEXROOT"));
		strcat(full_path, path.c_str());
		return full_path;
	}

	// void Initialize(py::array_t<float> scene_params, int thread_idx = 0)
	void Initialize(py::array_t<float> scene_params, 
                    py::array_t<float> vertices,
                    py::array_t<int> stretch_edges,
                    py::array_t<int> bend_edges,
                    py::array_t<int> shear_edges,
                    py::array_t<int> faces,
                    int thread_idx = 0)
	{
		auto ptr = (float *) scene_params.request().ptr;

		float radius = ptr[0];

		// rigid parameters
		float type = ptr[1];
		float dimx_rigid = ptr[2];
		float dimy_rigid = ptr[3];
		float dimz_rigid = ptr[4];
		float scale_rigid = ptr[5];
		float mass_rigid = ptr[6];
		float rotation = ptr[7];

		// cloth parameters
		float dimx_cloth = ptr[8];
		float dimy_cloth = ptr[9];
		float dimz_cloth = ptr[10];
		float stretchStiffness = ptr[11];
		float bendStiffness = ptr[12];
		float shearStiffness = ptr[13];
		float mass_cloth = ptr[14];
		float size_cloth_x = ptr[15];
		float size_cloth_z = ptr[16];
		float size_cloth_y = ptr[17];

		// others
		float dynamic_friction = ptr[18];
		float static_friction = ptr[19];
		float viscosity = ptr[20];
		float draw_mesh = ptr[21];

		float rigid_invMass = 1.0f/mass_rigid;
		int group = 0;
		float s = radius*0.5f;

		char path[100];

		if (type == 1)
			make_path(path, "/data/box.ply");
		else if (type == 3)
			make_path(path, "/data/ycb/03_cracker_box.obj");
		else if (type == 4)
			make_path(path, "/data/ycb/04_sugar_box.obj");
		else if (type == 5)
			make_path(path, "/data/ycb/05_tomato_soup_can.obj");
		else if (type == 6)
			make_path(path, "/data/ycb/06_mustard_bottle.obj");
		else if (type == 7)
			make_path(path, "/data/ycb/07_tuna_fish_can.obj");
		else if (type == 8)
			make_path(path, "/data/ycb/08_pudding_box.obj");
		else if (type == 9)
			make_path(path, "/data/ycb/09_gelatin_box.obj");
		else if (type == 10)
			make_path(path, "/data/ycb/10_potted_meat_can.obj");
		else if (type == 12)
			make_path(path, "/data/ycb/12_strawberry.obj");
		else if (type == 13)
			make_path(path, "/data/ycb/13_apple.obj");
		else if (type == 14)
			make_path(path, "/data/ycb/14_lemon.obj");
		else if (type == 15)
			make_path(path, "/data/ycb/15_peach.obj");	
		else if (type == 16)
			make_path(path, "/data/ycb/16_pear.obj");
		else if (type == 17)
			make_path(path, "/data/ycb/17_orange.obj");
		else if (type == 19)
			make_path(path, "/data/ycb/19_pitcher_base.obj");
		else if (type == 21)
			make_path(path, "/data/ycb/21_bleach_cleanser.obj");
		else if (type == 24)
			make_path(path, "/data/ycb/24_bowl.obj");
		else if (type == 25)
			make_path(path, "/data/ycb/25_mug.obj");
		else if (type == 35)
			make_path(path, "/data/ycb/35_power_drill.obj");
		else if (type == 36)
			make_path(path, "/data/ycb/36_wood_block.obj");
		else if (type == 37)
			make_path(path, "/data/ycb/37_scissors.obj");
		else if (type == 38)
			make_path(path, "/data/rigid/wiper.obj");
		
		CreateParticleShape(
		        GetFilePathByPlatform(path).c_str(),
				Vec3(dimx_rigid, dimy_rigid, dimz_rigid),
				scale_rigid, rotation, s, Vec3(0.0f, 0.0f, 0.0f), 
				rigid_invMass, true, 1.0, NvFlexMakePhase(group++, 0), true, 0.0f,
				0.0f, 0.0f, Vec4(0.0f), 0.0f, true);

		g_numSolidParticles = g_buffers->positions.size();

		int clothStart = g_buffers->positions.size();
		
		// add cloth
		auto verts_buf = vertices.request();
        size_t num_verts = verts_buf.shape[0] / 3;
		int phase = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseSelfCollideFilter);
        if (num_verts > 0)
        {
            // If a mesh is passed, then use passed in mesh
            float mass = float(mass_cloth) / num_verts;
            float invMass = 1.0f / mass;
            auto lower = Vec4(dimx_cloth, dimy_cloth, dimz_cloth, 0);

            int baseIndex = int(g_buffers->positions.size());
            Vec3 velocity = Vec3(0, 0, 0);

            auto verts_ptr = (float *)verts_buf.ptr;
            for (size_t idx = 0; idx < num_verts; idx++)
            {
                g_buffers->positions.push_back(
                    Vec4(verts_ptr[3 * idx], verts_ptr[3 * idx + 1], verts_ptr[3 * idx + 2], invMass) + lower);
                g_buffers->velocities.push_back(velocity);
                g_buffers->phases.push_back(phase);
            }

            auto faces_buf = faces.request();
            auto faces_ptr = (int *)faces_buf.ptr;
            size_t num_faces = int(faces_buf.shape[0] / 3);
            for (size_t idx = 0; idx < num_faces; idx++)
            {
                g_buffers->triangles.push_back(baseIndex + faces_ptr[3 * idx]);
                g_buffers->triangles.push_back(baseIndex + faces_ptr[3 * idx + 1]);
                g_buffers->triangles.push_back(baseIndex + faces_ptr[3 * idx + 2]);
                auto p1 = g_buffers->positions[baseIndex + faces_ptr[3 * idx]];
                auto p2 = g_buffers->positions[baseIndex + faces_ptr[3 * idx + 1]];
                auto p3 = g_buffers->positions[baseIndex + faces_ptr[3 * idx + 2]];
                auto U = p2 - p1;
                auto V = p3 - p1;
                auto normal = Vec3(
                    U.y * V.z - U.z * V.y,
                    U.z * V.x - U.x * V.z,
                    U.x * V.y - U.y * V.x);
                g_buffers->triangleNormals.push_back(normal / Length(normal));
            }
            
            auto stretch_edges_buf = stretch_edges.request();
            auto stretch_edges_ptr = (int *)stretch_edges_buf.ptr;
            size_t num_stretch_edges = int(stretch_edges_buf.shape[0] / 2);
            for (size_t idx = 0; idx < num_stretch_edges; idx++)
            {
                CreateSpring(baseIndex + stretch_edges_ptr[2 * idx], baseIndex + stretch_edges_ptr[2 * idx + 1], stretchStiffness);
            }

            auto bend_edges_buf = bend_edges.request();
            auto bend_edges_ptr = (int *)bend_edges_buf.ptr;
            size_t num_bend_edges = int(bend_edges_buf.shape[0] / 2);
            for (size_t idx = 0; idx < num_bend_edges; idx++)
            {
                CreateSpring(baseIndex + bend_edges_ptr[2 * idx], baseIndex + bend_edges_ptr[2 * idx + 1], bendStiffness);
            }

            auto shear_edges_buf = shear_edges.request();
            auto shear_edges_ptr = (int *)shear_edges_buf.ptr;
            size_t num_shear_edges = int(shear_edges_buf.shape[0] / 2);
            for (size_t idx = 0; idx < num_shear_edges; idx++)
            {
                CreateSpring(baseIndex + shear_edges_ptr[2 * idx], baseIndex + shear_edges_ptr[2 * idx + 1], shearStiffness);
            }
        }
        else
        {
            float mass = mass_cloth; // avg bath towel is 500-700g
            CreateSpringGrid(Vec3(dimx_cloth, dimy_cloth, dimz_cloth), size_cloth_x, size_cloth_z, size_cloth_y, radius, 
			NvFlexMakePhase(group++, 0), stretchStiffness, bendStiffness, shearStiffness, 0.0f, 1.0f / mass);
        }
		

		float restDistance = radius*0.55f;

		g_numSubsteps = 2;
		g_params.numIterations = 4;

		g_params.radius = radius;
		g_params.dynamicFriction = dynamic_friction;
		g_params.staticFriction = static_friction;
		g_params.viscosity = viscosity;

		g_params.dissipation = 0.01f;
		g_params.particleCollisionMargin = g_params.radius*0.05f;
		g_params.sleepThreshold = g_params.radius*0.25f;	
		g_params.damping = 0.25f;
		g_params.maxAcceleration = 400.0f;

		// g_params.particleCollisionMargin = g_params.radius*0.25f;	// 5% collision margin
		// g_params.sleepThreshold = g_params.radius*0.25f;
		// g_params.shockPropagation = 6.f; // aviod peneration for rigid
		// g_params.restitution = 0.2f; // aviod peneration for rigid
		// g_params.relaxationFactor = 1.f;
		// g_params.damping = 1.0f; //0.14f

		// Emitter e1;
		// e1.mDir = Vec3(1.0f, 0.0f, 0.0f);
		// e1.mRight = Vec3(0.0f, 0.0f, -1.0f);
		// e1.mPos = Vec3(radius, 1.f, 0.65f);
		// e1.mSpeed = (restDistance/g_dt)*2.0f; // 2 particle layers per-frame
		// e1.mEnabled = true;

		// g_emitters.push_back(e1);

		// g_waveFloorTilt = 0.0f;
		// g_waveFrequency = 1.5f;
		// g_waveAmplitude = 2.0f;
		
		g_warmup = false;

		if (draw_mesh) {
			g_drawMesh = true;
			g_drawPoints = false;
			g_drawSprings = false;
		} else {
			g_drawMesh = false;
			g_drawPoints = true;
			g_drawSprings = false;
		};
	}
	
};
