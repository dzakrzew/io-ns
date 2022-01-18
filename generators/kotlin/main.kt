package org.ions

class Adler32 {
    fun digest(input: String) {
        println(input)
    }
}

fun main() {
    val adler32 = Adler32()
    adler32.digest("aaa")
    println("Hello!")
}