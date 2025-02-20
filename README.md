# eVtol_Simulator

1.The idea is to have a base class called vehicle that defines all of the common characteristics among the different EVTOLs and then 
to have a series of subclasses that statically define the different qualities of the aircrafts developed by the five companies.

2.Later created a EVTOL factory to dynamically create the instances of the vehicles that will be tested in the simulator, it's best to proceed in this way because of the unknown
number of vehicles of each kind that have to be deployed each times the code is run.

3.Next a Chargers class has to be implemented, each charger needs a queue and attributes to determine availability.
A factory class to create the Chargers has also to be implemented.

4.Lastly creating a simulator class with a loop that emulates the passing of time at an accellerated rate, here are implemented the functions to record the flight info, 
a handler function for the charging queues and the fault events.

Todo: write a simple python script to visualize the data retrived from the simulator
