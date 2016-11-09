require_relative "helper"

class TestPairingParam < TestCase
  def test_to_and_init_from_str
    assert_raise RuntimeError do
      PairingParam.init_from_str("type a")
    end
    param = PairingParam.init_from_str(TYPE_A_PARAM)
    assert_equal TYPE_A_PARAM, param.to_str
  end

  def test_gen_type_a
    param = PairingParam.gen_type_a(160, 512)
    assert_match %r{type a}, param.to_str
  end
end
