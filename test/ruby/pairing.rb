require_relative "helper"

class TestPairing < TestCase
  def setup
    @param = PairingParam.init_from_str(TYPE_A_PARAM)
  end

  def test_init
    assert_raise RuntimeError do
      Pairing.init_from_param_str("type a")
    end
    pairing1 = Pairing.init_from_param(@param)
    pairing2 = Pairing.init_from_param_str(@param.to_str())
    assert_equal pairing1.symmetric, pairing2.symmetric
    assert_equal pairing1.g1_bytes_length, pairing2.g1_bytes_length
    assert_equal pairing1.g2_bytes_length, pairing2.g2_bytes_length
  end

  def test_info
    pairing = Pairing.init_from_param(@param)
    assert pairing.symmetric
    assert_equal 128, pairing.g1_bytes_length
    assert_equal 64, pairing.g1_x_only_bytes_length
    assert_equal 65, pairing.g1_compressed_bytes_length
    assert_equal 128, pairing.g2_bytes_length
    assert_equal 64, pairing.g2_x_only_bytes_length
    assert_equal 65, pairing.g2_compressed_bytes_length
    assert_equal 128, pairing.gt_bytes_length
    assert_equal 20, pairing.zr_bytes_length
  end
end
