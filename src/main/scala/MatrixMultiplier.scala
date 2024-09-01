package accelerator

import chisel3._
import chisel3.util._

class MatrixMultiplierIO(val SIZE: Int) extends Bundle {
  val start = Input(Bool())
  val done = Output(Bool())
  val a = Input(Vec(SIZE * SIZE, UInt(8.W)))
  val b = Input(Vec(SIZE * SIZE, UInt(8.W)))
  val c = Output(Vec(SIZE * SIZE, UInt(16.W)))
}

class MatrixMultiplier(val SIZE: Int = 8) extends Module {
  val io = IO(new MatrixMultiplierIO(SIZE))

  val a_reg = RegInit(VecInit(Seq.fill(SIZE * SIZE)(0.U(8.W))))
  val b_reg = RegInit(VecInit(Seq.fill(SIZE * SIZE)(0.U(8.W))))
  val c_reg = RegInit(VecInit(Seq.fill(SIZE * SIZE)(0.U(16.W))))

  val idle :: computing :: done :: Nil = Enum(3)
  val state = RegInit(idle)

  val i = RegInit(0.U(log2Ceil(SIZE).W))
  val j = RegInit(0.U(log2Ceil(SIZE).W))
  val k = RegInit(0.U(log2Ceil(SIZE).W))

  when(state === idle && io.start) {
    a_reg := io.a
    b_reg := io.b
    state := computing
  }

  when(state === computing) {
    when(k === (SIZE - 1).U) {
      k := 0.U
      when(j === (SIZE - 1).U) {
        j := 0.U
        when(i === (SIZE - 1).U) {
          state := done
        }.otherwise {
          i := i + 1.U
        }
      }.otherwise {
        j := j + 1.U
      }
    }.otherwise {
      k := k + 1.U
    }

    val product = a_reg(i * SIZE.U + k) * b_reg(k * SIZE.U + j)
    c_reg(i * SIZE.U + j) := c_reg(i * SIZE.U + j) + product
  }

  when(state === done) {
    state := idle
    i := 0.U
    j := 0.U
    k := 0.U
  }

  io.done := state === done
  io.c := c_reg
}