# IR COM

Embedded Autonomous Systems - Project

Participants: 
* Andreas Salzmann
* Daniel Caruso
* Jürgen Markl

WS 2022/23, HAW Landshut


## Scope

In this repository, the firmware for the MSP430FR5959 will be developed, which should act as a battery montiring device and communicates it's data to a collecting instance using a infrared interface. 

As the underlying protocoll, the IrDA-Stack shall be implemented using the IrDA lite version, as the full stack would contain too much features, that are not needed for this simple communication. 

## Project structure

in library_dev the IrDA Stack will be developed

in msp430_workspace, a workspace for the Code Composer Studio is created and the integration will happen in there


