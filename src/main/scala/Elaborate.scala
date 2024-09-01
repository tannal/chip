package accelerator

import chisel3.stage.ChiselStage

object Elaborate extends App {
  (new ChiselStage).emitVerilog(new MatrixMultiplier(), Array("--target-dir", "generated"))
}