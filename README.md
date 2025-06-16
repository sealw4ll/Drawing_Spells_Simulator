# Drawing_Spells_Simulator
## installation
Using Visual Studio 2022
1. clone and open Visual Studio 2022
2. vcpkg install
3. build all
4. run game

Using cmd
1. clone repo
2. vcpkg install
3. ```cmake -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake ..```
4. ```cmake -B build .```
5. ```cmake --build build --config Release```
6. ```./build/Drawing_Spells_Simulator```

# How to Cast Spells
Levels are casted based on 2 identifiers
1. spell level
2. damage type

You can add spells in the spells folder as a json file. The program will automatically parse it. Any errors found the spell just wont be present in the game. 

## cantrip spells
Casting Fire Bolt

![image](https://github.com/user-attachments/assets/fa2bcac4-5df1-4984-9ea5-c25e46847a53)


## level 1 spells
Casting Thunderwave

![image](https://github.com/user-attachments/assets/13e211f0-b478-4890-b13d-ae8123d4d3b5)

## level 2 spells
Casting Shatter

![image](https://github.com/user-attachments/assets/1186ab06-88d2-49e1-a1ff-a0afd0f71158)


## level 3 spells
Casting Fireball

![image](https://github.com/user-attachments/assets/4bc89525-b405-4ac5-975c-981d120a6013)

Casting Lightning Bolt

![image](https://github.com/user-attachments/assets/62d40643-2c31-4316-b0ec-189c6e2c1158)


## level 4 spells
Casting Ice Storm

![image](https://github.com/user-attachments/assets/0aca2967-26e3-415c-bdaa-d1cc8f640ad9)


## level 5 spells
Casting Cone Of Cold

![image](https://github.com/user-attachments/assets/21043519-4749-46ce-bcf7-9462bcf00f23)

Casting Telekinesis 

![image](https://github.com/user-attachments/assets/87c0a7c8-be78-4d1e-958b-a1f01827f829)
