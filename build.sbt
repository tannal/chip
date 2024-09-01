scalaVersion := "2.12.13"
addCompilerPlugin("edu.berkeley.cs" % "chisel3-plugin" % "3.4.3" cross CrossVersion.full)
libraryDependencies += "edu.berkeley.cs" %% "chisel3" % "3.4.3"
libraryDependencies += "edu.berkeley.cs" %% "chiseltest" % "0.3.3" % "test"
