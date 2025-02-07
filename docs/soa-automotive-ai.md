# State of Art of AI in automotive

# Autonomous Vehicles

## Automated Car Systems

According to the Society of Automotive Engineers (SAE international), driving automation can be categorized into six levels (SAE J3016):

| Level | Name | Driver | Driving Environment Monitoring (DEM) | Dynamic Driving Task Fallback (DDTF) |
| --- | --- | --- | --- | --- |
| 0 | No automation | Human | Human | Human |
| 1 | Driver assistance | Human & System | Human | Human |
| 2 | Partial automation | System | Human | Human |
| 3 | Conditional automation | System | System | Human |
| 4 | High automation | System | System | System |
| 5 | Full automation | System | System | System |

The state of art Automated Driving Systems (ADS) are mainly at level 2 and 3. Vehicles should satisfy at least the functions described by this standard in order to qualify for automation. 

Some of the most important terms in this standard are:

- **Dynamic Driving Task (DDT):** real-time operational (steerin, breakin) and tactical (lane keeping) functions required to operate a vehicle.
- **Driving Automation System:** hardware and software systems collectively capable of performing some or all parts of the DDT on a sustained basis.
- **Operational Design Domains (ODD):** the specific conditions under which a given driving automation system or feature is designed to function. It might be every conditions or something narrower, such as a floor road mat, changing the associated requirements.
- **DDT Fallback:** the response by the user or by an *Automated Driving System* (ADS) to perform a DDT task in case of system failure.
- **DDT fallback-ready-user:** the user of a vehicle who is able to operate the vehicle and intervene if necessary
- **DDT feature:** design-specific functionality at a specific level of driving automation with a particular ODD (ex: lane keeping in sunny weather).

## Automated Car Software

Rui Fan et all divides the software architecture of Autonomous Cars (AC) into five modules: 

- **Perception**: analyzes raw sensor data (such as cameras, Radar, Lidar) and outputs into something that can be used by the AC. This can be broken into computer vision-based (optimization techniques) and machine learning-based (data-driven classification or regression - CNN).
- **Localization and Mapping**: Using sensor data and perception output to estimate AC location and build/update 3D world map.
- **Prediction**: analyzes motion patterns of other traffic agents and predict future AC trajectories
- **Planning**: determines possible safe AC navigation routes based on all the previous modules’ information.
- **Control**: sends the appropriate commands to throttle, braking or steering torque, based on the predicted trajectory and the estimated vehicle states, enabling the AC to follow the planned trajectory.

# Autonomous Lane Keeping Systems

## Related Works

Traditional Machine Learning methods have been used with some success in the field of ALKS, with varying degrees of success (Bayesian Classifier, Haar Cascades, Extreme Learning Machine (ELM), Support Vector Machine (SVM), and Artificial Neural Network (ANN)), although Deep Learning methods have been gaining more popularity due to their effective performance in either classification or detection [Systematic Review on ALSK](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=10006813). 

## Legal Considerations

### UN-ECE regulation 157

UN Regulation No. 157, established by the United Nations Economic Commission for Europe (UNECE), sets uniform provisions for the approval of vehicles equipped with Automated Lane Keeping Systems (ALKS). ALKS are advanced systems that enable a vehicle to operate autonomously within its lane on specific road types, such as motorways, under defined conditions.

**Key Features of UN Regulation No. 157:**

- **Scope and Application:** Initially, the regulation permitted ALKS operation at speeds up to 60 km/h, primarily targeting congested traffic scenarios. Subsequent amendments have increased this limit to 130 km/h, allowing for broader application, including higher-speed motorway driving. [interregs.com](https://www.interregs.com/articles/spotlight/252/updated-un-ece-regulation-on-automated-lane-keeping-systems-published-)
- **System Requirements:** The regulation mandates that ALKS-equipped vehicles must ensure the driver is seated, wearing a seatbelt, and available to take over control when necessary. The system should function on roads with physical separation between opposing traffic directions and be capable of handling various weather conditions. [en.wikipedia.org](https://en.wikipedia.org/wiki/Automated_lane_keeping_systems)
- **Transition Demand:** ALKS must provide a clear and intuitive procedure to transfer control back to the driver, known as a "transition demand." This involves alerting the driver and allowing a sufficient period, typically 10 seconds, for the driver to resume manual control. [en.wikipedia.org](https://en.wikipedia.org/wiki/Automated_lane_keeping_systems)
- **Data Storage System for Automated Driving (DSSAD):** Vehicles must be equipped with a DSSAD to record essential data related to the automated driving functions. This ensures accountability and aids in incident investigations. [en.wikipedia.org](https://en.wikipedia.org/wiki/Automated_lane_keeping_systems)
- **Safety and Collision Avoidance:** The regulation outlines requirements for collision avoidance, ensuring that ALKS can handle specific scenarios to maintain safety. [en.wikipedia.org](https://en.wikipedia.org/wiki/Automated_lane_keeping_systems)