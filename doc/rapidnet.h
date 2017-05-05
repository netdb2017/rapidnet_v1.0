/**
 * \page rapidnet RapidNet
 *
 * \section rapidnet-intro Introduction
 * RapidNet is a development toolkit for rapid simulation and implementation of
 * network protocols. RapidNet integrates a declarative networking engine with
 * ns-3 and thus provides it a declarative interface. Protocols in RapidNet are
 * written in a declarative programming language called <i>NDlog</i>, which
 * stands for <i>Network Datalog</i>. These protocols are compiled using the
 * \ref rapidnet-compiler into C++ code that runs over the \ref
 * rapidnet-library.
 *
 * Current version of RapidNet is implemented over version ns-3.5.
 *
 * \section rapidnet-compiler RapidNet Compiler
 * The RapidNet compiler parses the input NDlog protocols and translates them
 * into an intermediate form of event-condition-action (ECA) rules. This
 * representation of the protocol is compiled into C++ code that runs as an
 * application over ns-3.
 *
 *
 * \section rapidnet-library RapidNet Library
 * The RapidNet library is an extension over ns-3. It implements the common
 * utilities across all RapidNet applications like the database engine,
 * database operators, handling of network events, the type system, expression
 *  trees and so on.
 *
 * Refer to the <a href="modules.html">Modules</a> page for API documentation
 * on the RapidNet library and compiler.
 *
 * \section rapidnet-files List of Files and Folders
 * Following source folders are added to the base ns-3 for RapidNet:
 *
 *    - <b>src/rapidnet</b>: RapidNet library classes.
 *         - <b>values</b>: RapidNet value types.
 *         - <b>tests</b>: RapidNet unit test-cases.
 *    - <b>src/rapidnet-compiler</b>: RapidNet compiler classes.
 *
 * RapidNet adds a few example scripts in the <b>examples</b> folder.
 * Additionally, a few Python scripts are added to the following folders
 * for:
 *
 *    - <b>rapidnet</b>: Invoking the compiler, generating graphs etc.
 *    - <b>simulation</b>: Running simulation experiments.
 *    - <b>emulation</b>: Running emulation experiments.
 *
 * \section rapidnet-all-applications Sample Applications
 * Several sample RapidNet applications are included in the release:
 *
 *  - Ping Pong: src/applications/pingpong
 *  - Neighbor Discovery: src/applications/discovery
 *  - All-pairs Reachable: src/applications/reachable
 *  - Path-vector: src/applications/pathvector
 *  - Link State Periodic: src/applications/sim-ls-periodic
 *  - Link State Periodic+Triggered: src/applications/sim-ls-triggered
 *  - HSLS Periodic: src/applications/sim-hsls-periodic
 *  - HSLS Periodic+Triggered: src/applications/sim-hsls-triggered
 *  - DSR: src/applications/dsr
 *  - Epidemic: src/applications/epidemic
 *  - Chord DHT: src/applications/chord
 *
 * \section rapidnet-examples Example Experiments
 * The examples folder contains a few experiments to run a few of the above
 * protocols:
 *
 *  - Ping Pong: examples/pingpong-test.cc
 *  - All-pairs Reachable: src/applications/reachable-test.cc
 *  - Path vector: src/applications/pathvector-test.cc
 *
 * It also includes a generic runner script to simulate any protocol under
 * different experimental settings. This script is:
 * <b>examples/rapidnet-app-simulator.cc</b>. A similar script for emulation
 * is <b>examples/rapidnet-app-emulator.cc</b>. See \ref rapidnet-howtos
 * for documentation on these scripts.
 *
 * Refer to \ref rapidnet-howtos for getting started with RapidNet.
 */
