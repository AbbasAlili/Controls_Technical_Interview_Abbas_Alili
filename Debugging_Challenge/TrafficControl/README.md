## Fixed Issues
The original program had three key issues that have been resolved:

### **Issue 1: Logic Error in `setHorizantalTrafficLight`**
- **Problem**: Missing `break` statements in the `switch` block caused unintended fallthrough behavior.
- **Fix**: Added `break` statements for each `case` in the `switch` block to ensure proper state transitions.

### **Issue 2: Incorrect Traffic Light State Initialization in `setVerticalTrafficLight`**
- **Problem**: The logic for detecting traffic light states (`RED`, `GREEN`, `YELLOW`) was nested incorrectly, leading to errors in assigning states.
- **Fix**: Rewrote the logic to correctly handle state transitions and detect the current state.

### **Issue 3: Crash Detection Logic in `checkForCrashes`**
- **Problem**: The logic incorrectly checked the same lanes for crashes, ignoring the vertical lanes.
- **Fix**: Adjusted the crash detection to properly check both vertical (`northbound`, `southbound`) and horizontal (`eastbound`, `westbound`) lanes.




## Ways to Improve Code
1. **Avoid Hardcoded Values**:
   - Replace hardcoded values like `"R"`, `"G"`, and `"Y"` with `enum` or defined constants for better readability and maintainability.
   ```c
   typedef enum { RED, GREEN, YELLOW } traffic_light_state_t;
   ```

2. **Ensure Proper Use of `break` Statements**
- Always include `break` statements in `switch` cases to prevent unintended fallthrough behavior, unless explicitly required and well-documented.



3. **Add Comments for Complex Logic**
- Use clear and descriptive comments to explain complex or non-intuitive logic, especially in critical functions like `setHorizantalTrafficLight` and `checkForCrashes`, to improve code maintainability.



## Future Enhancements

### Flexibility:

Easily update parameters like traffic light durations, lane popularity, or maximum car counts without touching the code.


### Add Support for Different Intersection Layouts
- Expand the program to handle custom intersection layouts, such as:
  - T-junctions.
  - Roundabouts.
  - Complex multi-lane intersections.
  - Realistic Modeling: Mimic real-world scenarios like rush hour congestion, enabling better traffic management strategies.


