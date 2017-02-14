require_relative "helper"

class TestElementPower < TestCase
  def setup
    @pairing = Pairing.init_from_param_str(TYPE_A_PARAM)
  end

  def test_element_power
    base = Element.new(@pairing, ElementType_G1)
    e = Element.new(@pairing, ElementType_G1)
    z = Element.new(@pairing, ElementType_Zr)
    base.random
    e.random
    z.random
    assert_raise RuntimeError do
      FixedBasePower.new Element.new
    end
    p = FixedBasePower.new base
    assert_equal base, p.get_base
    assert_raise RuntimeError do
      p.apply Element.new
    end
    assert_raise ArgumentError do
      p.apply e
    end
    assert_equal base ** z, p.apply(z)
    assert_equal base ** 2, p.apply(2)
    assert_equal base ** z, p ** z
    assert_equal base ** 2, p ** 2
  end

  def test_element_pow2
    a1 = Element.new(@pairing, ElementType_G1)
    a2 = Element.new(@pairing, ElementType_G1)
    n1 = Element.new(@pairing, ElementType_Zr)
    n2 = Element.new(@pairing, ElementType_Zr)
    a1.random
    a2.random
    n1.random
    n2.random
    assert_raise RuntimeError do
      pow2 Element.new, Element.new, Element.new, Element.new
    end
    assert_raise ArgumentError do
      pow2 a1, a1, a2, a2
    end
    assert_equal pow2(a1, n1, a2, n2), a1.pow(n1) * a2.pow(n2)
  end

  def test_element_pow3
    a1 = Element.new(@pairing, ElementType_G1)
    a2 = Element.new(@pairing, ElementType_G1)
    a3 = Element.new(@pairing, ElementType_G1)
    n1 = Element.new(@pairing, ElementType_Zr)
    n2 = Element.new(@pairing, ElementType_Zr)
    n3 = Element.new(@pairing, ElementType_Zr)
    a1.random
    a2.random
    a3.random
    n1.random
    n2.random
    n3.random
    assert_raise RuntimeError do
      pow3 Element.new, Element.new, Element.new, Element.new, Element.new, Element.new
    end
    assert_raise ArgumentError do
      pow3 a1, a1, a2, a2, a3, a3
    end
    assert_equal pow3(a1, n1, a2, n2, a3, n3), a1.pow(n1) * a2.pow(n2) * a3.pow(n3)
  end
end
