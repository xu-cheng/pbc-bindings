require "test/unit"
require "pbc"

class TestCase < Test::Unit::TestCase
  include Pbc

  TYPE_A_PARAM = File.read("./a.param").freeze
end
