# eScooterSIM

### Welcome to eScooterSIM, a VR driving simulator for behavioral and interactions research.

This project extends the [`DReyeVR`](https://github.com/HARPLab/DReyeVR.git) simulator to add virtual reality integration, a first-person maneuverable Electric Scooter with Arduino Steering Controller support, and several immersion enhancements.

If you have questions, hopefully, our [F.A.Q. wiki page](https://github.com/HARPLab/DReyeVR/wiki/Frequently-Asked-Questions) and [issues page](https://github.com/HARPLab/DReyeVR/issues?q=is%3Aissue+is%3Aclosed) can answer some of them.

**IMPORTANT:** Currently eScooterSIM only supports Carla versions: [0.9.13](https://github.com/carla-simulator/carla/tree/0.9.13) with Unreal Engine 4.26

## Highlights

### Ego Vehicle

Fully drivable **virtual reality (VR) Electric Scooter** with [SteamVR integration](https://github.com/ValveSoftware/steamvr_unreal_plugin/tree/4.23) (see [EgoVehicle.h](DReyeVR/EgoVehicle.h))

- SteamVR HMD head tracking (orientation & position)

  - We have tested with the following devices:| Device                                      | VR Supported | Eye tracking | OS             |
    | ------------------------------------------- | ------------ | ------------ | -------------- |
    | [Varjo XR-3](https://varjo.com/products/xr-3/) | ✅           | ❌           | Windows, Linux |
  - While we haven't tested other headsets, they should still work for basic VR usage (not eye tracking) if supported by SteamVR.
- Vehicle controls:

  - Generic keyboard WASD + mouse
  - Support for Logitech Steering wheel with this open source [LogitechWheelPlugin](https://github.com/HARPLab/LogitechWheelPlugin)

    - Includes force feedback with the steering wheel.
    - We used a [Logitech G923 Racing Wheel &amp; Pedals](https://www.logitechg.com/en-us/products/driving/driving-force-racing-wheel.html)
      - Full list of supported devices can be found [here](https://github.com/HARPLab/LogitechWheelPlugin/blob/master/README.md) though we can't guarantee out-of-box functionality without testing.
  - Support for Arduino Due-based Steering controller with this open source [Unreal_Engine_SerialCOM_Plugin](https://github.com/videofeedback/Unreal_Engine_SerialCOM_Plugin.git)

    - We used a [Arduino Due board](https://store.arduino.cc/products/arduino-due)
      - It is supported on all Windows OS (on Linux, not tested), though we can't guarantee out-of-box functionality without testing.
- eScooter dashboard:

  - Speedometer (in miles-per-hour by default)
  - Gear indicator
  - Turn signals
- "Ego-centric" audio

  - Responsive engine revving (throttle-based)
  - Turn signal clicks
  - Gear switching
  - Collisions
- Fully compatible with the existing Carla [PythonAPI](https://carla.readthedocs.io/en/0.9.13/python_api/) and [ScenarioRunner](https://github.com/carla-simulator/scenario_runner/tree/v0.9.13)

  - Minor modifications were made. See [Usage.md](Docs/Usage.md) documentation.
- Fully compatible with the Carla [Recorder and Replayer](https://carla.readthedocs.io/en/0.9.13/adv_recorder/)

  - Including HMD pose/orientation & sensor reenactment
- Ability to handoff/takeover control to/from Carla's AI wheeled vehicle controller
- Carla-based semantic segmentation camera (see [`Shaders/README.md`](Shaders/README.md))

### Ego Sensor

Carla-compatible **ego-vehicle sensor** (see [EgoSensor.h](DReyeVR/EgoSensor.h)) is an "invisible sensor" that tracks the following:

- Real-time user inputs (throttle, steering, brake, turn signals, etc.)
- Image (screenshot) frame capture based on the camera
  - Typically used in Replay rather than real-time because highly performance intensive.
- Fully compatible with the LibCarla data serialization for streaming to a PythonAPI client (see [LibCarla/Sensor](LibCarla/Sensor))
  - We have also tested and verified support for (`rospy`) ROS integration in our sensor data streams

### Other additions:

- Custom DReyeVR config file for one-time runtime params. See [DReyeVRConfig.ini](Configs/DReyeVRConfig.ini)
  - Especially useful to change params without recompiling everything.
  - Uses standard C++ io management to read the file with a minimal performance impact. See [DReyeVRUtils.h](DReyeVR/DReyeVRUtils.h).
- World ambient audio
  - Birdsong, wind, smoke, etc. (See [Docs/Sounds.md](Docs/Sounds.md))
- Non-ego-centric audio (Engine revving from non-ego vehicles)
- Synchronized Replay with per-frame frame capture for post-hoc analysis (See [Docs/Usage.md](Docs/Usage.md))
- Recorder/replayer media functions
  - Added in-game keyboard commands Play/Pause/Forward/Backward/etc.
- Static in-environment directional signs for natural navigation (See [`Docs/Signs.md`](Docs/Signs.md))
- Adding weather to the Carla recorder/replayer/query (See this [Carla PR](https://github.com/carla-simulator/carla/pull/5235))
- Custom dynamic 3D actors with full recording support (e.g., HUD indicators for direction, AR bounding boxes, visual targets, etc.). See [CustomActor.md](Docs/CustomActor.md) for more.
- (DEBUG ONLY) Foveated rendering for improved performance with gaze-aware (or fixed) variable rate shading

## Install/Build

See [`Docs/Install.md`](Docs/Install.md) to:

- Install and build `DReyeVR` on top of a working `Carla` repository.
- Download plugins for `DReyeVR` required for fancy features such as:
  - [Unreal_Engine_SerialCOM_Plugin]()
  - Steering wheel/pedals (Logitech)
- Set up a `conda` environment for DReyeVR PythonAPI

## OS compatibility

| OS      | VR | Eye tracking | Audio | Keyboard+Mouse | Racing wheel | Foveated Rendering (Editor) |
| ------- | -- | ------------ | ----- | -------------- | ------------ | --------------------------- |
| Windows | ✅ | ❌           | ✅    | ✅             | ✅           | ✅                          |
| Linux   | ❌ | ❌           | ❌    | ❌             | ❌           | ❌                          |
| MacOS   | ❌ | ❌           | ❌    | ❌             | ❌           | ❌                          |

- While Windows (10) is recommended for optimized VR support, all our work translates to Linux systems except for the eye tracking and hardware integration, which have Windows-only dependencies.
  - Unfortunately, the eye-tracking firmware is proprietary & does not work on Linux
    - This is (currently) only supported on Windows because of some proprietary dependencies between [HTC SRanipal SDK](https://developer.vive.com/resources/knowledgebase/vive-sranipal-sdk/) and Tobii's SDK. Those interested in the Linux discussion for HTC's Vive Pro Eye Tracking can follow the subject [here (Vive)](https://forum.vive.com/topic/6994-eye-tracking-in-linux/), [here (Vive)](https://forum.vive.com/topic/7012-vive-pro-eye-on-ubuntu-16-or-18/), and [here (Tobii)](https://developer.tobii.com/community/forums/topic/vive-pro-eye-with-stream-engine/).
  - Additionally, the [LogitechWheelPlugin](https://github.com/HARPLab/LogitechWheelPlugin) we use only has Windows support currently. However, it should be possible to use the G923 on Linux as per the [Arch Wiki](https://wiki.archlinux.org/title/Logitech_Racing_Wheel).
- Also, although MacOS is not officially supported by CARLA, we have development happening on an Apple Silicon machine and have active forks of CARLA + UE4.26 with MacOS 12+ support. Note that this is primarily for development, as it is the most limited system by far.

## Documentation & Guides

- See [`F.A.Q. wiki`](https://github.com/HARPLab/DReyeVR/wiki/Frequently-Asked-Questions) for our Frequently Asked Questions wiki page.
- See [`Usage.md`](Docs/Usage.md) to learn how to use our provided DReyeVR features
- See [`Development.md`](Docs/Development.md) to get started with DReyeVR development and add new features
- See [`Sounds.md`](Docs/Sounds.md) to use our custom sounds to CARLA and create your own
- See [`CustomActor.md`](Docs/CustomActor.md) to use our CustomActor API and spawn "Overlay" actors
- See [`Model.md`](Docs/Model.md) to learn how we added a dynamic steering wheel to the ego vehicle
- See [`Signs.md`](Docs/Signs.md) to add custom in-world directional signs and spawn them
- See [`LODs.md`](Docs/LODs.md) to learn about LOD modes for tuning your VR experience

<!-- - See [`SetupVR.md`](Docs/SetupVR.md) to learn how to quickly and minimally set up VR with Carla -->

## Citation

If you use our work, please cite the corresponding [paper](https://arxiv.org/abs/2201.01931):

```bibtex
@inproceedings{silvera2022dreyevr,
  title={DReyeVR: Democratizing Virtual Reality Driving Simulation for Behavioural \& Interaction Research},
  author={Silvera, Gustavo and Biswas, Abhijat and Admoni, Henny},
  booktitle={Proceedings of the 2022 ACM/IEEE International Conference on Human-Robot Interaction},
  pages={639--643},
  year={2022}
}
```

## Acknowledgements

- This project builds upon and extends the [DReyeVR](https://github.com/HARPLab/DReyeVR.git)
- This project builds upon and extends the [CARLA simulator](https://carla.org/)
- This repo includes some code from CARLA: Computer Vision Center (CVC) at the Universitat Autonoma de Barcelona (UAB) & Intel Corporation.
- This repo includes some code from Hewlett-Packard Development Company, LP. See [nvidia.ph](Tools/Diagnostics/collectl/nvidia.ph). This is a modified diagnostic tool used during development.

## Licenses

- Custom DReyeVR code is distributed under the MIT License.
- Unreal Engine 4 follows its [own license terms](https://www.unrealengine.com/en-US/faq).
- Code used from other sources that is prefixed with a Copyright header belongs to those individuals/organizations.
- CARLA specific licenses (and dependencies) are described on their [GitHub](https://github.com/carla-simulator/carla#licenses)
