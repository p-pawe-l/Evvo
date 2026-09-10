#pragma once 


namespace evvo::sampling {

template <typename SampleT>
class AbstractSampler {
public:
    virtual ~AbstractSampler() = default; 
    [[nodiscard]] virtual SampleT get_sample() const;
};

}