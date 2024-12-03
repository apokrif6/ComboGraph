# ComboGraph

The `ComboGraph` plugin which provides a system for defining and handling combo sequences in Unreal
Engine. It brings new fully functional blueprint-based graph to create complex combo chains.

Main idea is not creating new production-ready solution, but rather to provide core functionality for developers who
wants to build their own system.

---

## 🌟 Features

- Define combo sequences using nodes in a blueprint-based graph.
- Handle input actions to trigger combos.
- Play animations for each combo step.
- Support for different types of combo nodes (Opener, Midler, Ender).

## 🚀 Installation

- Clone or download the `ComboGraph` plugin into your project's `Plugins` directory.
- Open your project in Unreal Engine.
- Enable the `ComboGraph` plugin in the Plugins window.

## ⚔️ Usage

How to properly setup system:

- Create a new `ComboGraph` and define your combo sequences with nodes.
- Every node should have set `InputAction` and `ComboMontage` properties. Montage also should have set `UCanContinueComboNotifyState` which will be window to continue combo.
- Implement `IComboHandlerInterface` for your character.
- Set the `ComboGraph` and `ComboInputMappingContext` properties on the `UComboHandlerComponent`.
- Now you can try to use it.

## 📙 Examples

Those are examples of combo sequences implemented with `ComboGraph` plugin

![ComboGraph Example 1](/Resources/Documentation/ComboGraphExample1.png)

![ComboGraph Example 2](/Resources/Documentation/ComboGraphExample2.png)

## 🎬 Showcase

[![Showcase video](https://img.youtube.com/vi/tODbzmV0z-w/0.jpg)](https://youtu.be/tODbzmV0z-w)

## 📄 Technical Documentation

### Core

Functionality is separated into two main modules: `ComboGraphEditor` and `ComboGraphRuntime`.

First one is responsible for editor-related logic, like creating nodes and connections.
It also handle basic commands, drag and drop, and other editor-related stuff.

Second one is responsible for runtime logic, like handling input actions and playing animations.

### Combo Graph Node

The `UComboGraphNode` class is the parent class for all combo sequence nodes. It includes properties for input actions,
animations, and connections to other nodes.

### Combo Node Types

Combo nodes are defined using the `EComboNodeType` enum:

- `None`: No specific type.
- `Opener`: The starting node of a combo.
- `Midler`: A middle node in a combo sequence.
- `Ender`: The ending node of a combo.

By default, connection policy is predefined, but can be overriden with `UComboGraphEnderNode::CanCreateConnectionFrom` and `UComboGraphNode::CanCreateConnectionTo`

### Connection Policy Table

| **Node Type** | **Can Connect From**             | **Can Connect To**             |
|---------------|----------------------------------|--------------------------------|
| **Opener**    | Can't be connected from anything | Midler                         |
| **Midler**    | Opener, Midler                   | Midler, Ender                  |
| **Ender**     | Midler                           | Can't be connected to anything |

### ComboHandlerComponent

The `UComboHandlerComponent` class handles the logic for processing combo sequences. It binds input actions, finds
suitable combo nodes, and plays animations.
