엔진 사용시 직접 추가해야하는것들

1.  셰이더 및 리소스 추가시 "FJSystemEngine.cpp" 에서 LoadData() 함수에서 생성
    셰이더의 상수버퍼는 Matrix->Vector->Scalar 순으로 구조체를 구성해야함
    셰이더의 상수버퍼의 구성요소를 셰이더 생성시 초기화 필요

2.  컴포넌트 추가시 "CustomComponent.h"에 자신이 추가한 컴포넌트의 헤더를 넣고 "Object.cpp"의 CObject::CopyObject(const CObject* origin) (line : 251) 함수 안에 자신의 컴포넌트를
    else if (ti == typeid(YourComponent))
    {
        cp = obj->AddComponent<Renderer>();
        memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(YourComponent), (char*)(*iter) + sizeof(Component), COPY_SIZE(YourComponent));
    }
    형식으로 추가해야함