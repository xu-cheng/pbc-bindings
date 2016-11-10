require_relative "helper"

class TestElementPower < TestCase
  def setup
    @pairing = Pairing.init_from_param_str(TYPE_A_PARAM)
    @base = Element.new(@pairing, ElementType_G1)
    @e = Element.new(@pairing, ElementType_G1)
    @z = Element.new(@pairing, ElementType_Zr)
    @base.random
    @e.random
    @z.random
  end

  def test_element_power
    assert_raise RuntimeError do
      FixedBasePower.new Element.new
    end
    p = FixedBasePower.new @base
    assert_raise RuntimeError do
      p.apply Element.new
    end
    assert_raise ArgumentError do
      p.apply @e
    end
    assert_equal @base ** @z, p.apply(@z)
    assert_equal @base ** 2, p.apply(2)
  end
end
