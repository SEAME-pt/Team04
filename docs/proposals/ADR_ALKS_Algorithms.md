# ADR: Automated Lane Keeping System

## Status

Proposed (draft decision, needs review)

## Context

We are working on a Automated Lane Keeping System (ALKS) that should be capable of detecting and tracking lane markings using simulated sensors. It should also simulate corrective interventions, such as steering or braking adjustments, to ensure the virtual vehicle remains in its lane,

## Proposal

For the Lane Keeping system, we are going to use the CLRerNet model, which is the state of art in lane detection.

For the corrective intervention of the car, we are going to use the Model Predictive Control (MPC) algorithm.

## Future Considerations:

- As the tests progress, if necessary, these may change.