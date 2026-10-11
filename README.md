# kal_d98: teknős mozgatása és pozíciójának naplózása

ROS 2 C++ package a "Autonóm járművek és robotok programozása" tárgy kis beadandójához.
[![Static Badge](https://img.shields.io/badge/ROS_2-Humble-34aec5)](https://docs.ros.org/en/humble/)

Szerző: Káldy Tamás

## Leírás

A package két node-ból áll, amelyek a `turtlesim` szimulátorral dolgoznak együtt:

- **`mover_node`** (publisher): `geometry_msgs/msg/Twist` üzeneteket küld a `/turtle1/cmd_vel` topicra. A teknős egy négyzet alakú pályát jár be: 2 másodpercig egyenesen megy, majd kb. 90 fokot fordul, és ezt ismétli.
- **`pose_logger_node`** (subscriber): feliratkozik a `/turtle1/pose` topicra (`turtlesim/msg/Pose`), és másodpercenként egyszer kiírja a teknős `x`, `y` és `theta` értékét.

## Függőségek

- ROS 2 Humble
- `turtlesim` (`sudo apt install ros-humble-turtlesim`, ha nincs telepítve)

## Build

Feltételezzük, hogy a workspace a `~/ros2_ws/`.

```bash
cd ~/ros2_ws/src
git clone https://github.com/kaldytamas/kal_d98
cd ~/ros2_ws
colcon build --packages-select kal_d98 --symlink-install
```

## Futtatás

Ne felejtsünk source-olni a ROS parancsok előtt.

```bash
source ~/ros2_ws/install/setup.bash
ros2 launch kal_d98 turtle.launch.py
```

A launch fájl egyszerre indítja a `turtlesim_node`-ot, a `mover_node`-ot és a `pose_logger_node`-ot. Leállítani `Ctrl+C`-vel lehet.

## Node-topic kapcsolatok

```mermaid
flowchart LR
    M([mover_node]):::red -->|geometry_msgs/msg/Twist| C["/turtle1/cmd_vel"]:::light
    C --> T([turtlesim_node]):::red
    T -->|turtlesim/msg/Pose| P["/turtle1/pose"]:::light
    P --> L([pose_logger_node]):::red

    classDef light fill:#34aec5,stroke:#152742,stroke-width:2px,color:#152742
    classDef red fill:#ef4638,stroke:#152742,stroke-width:2px,color:#fff
```

| Node | Publikál | Feliratkozik |
|---|---|---|
| `mover_node` | `/turtle1/cmd_vel` (`geometry_msgs/msg/Twist`) | – |
| `turtlesim_node` | `/turtle1/pose` (`turtlesim/msg/Pose`) | `/turtle1/cmd_vel` |
| `pose_logger_node` | – | `/turtle1/pose` |

## Eredmény

A teknős mozgása a szimulátorban:

![A teknős útja](img/turtle_path.png)

A `pose_logger_node` kimenete:

![A pozíció naplózása](img/terminal_output.png)

## Fájlok

```
kal_d98/
├── CMakeLists.txt
├── package.xml
├── launch/
│   └── turtle.launch.py
├── src/
│   ├── mover_node.cpp
│   └── pose_logger_node.cpp
└── img/
```