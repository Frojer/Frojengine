���� ���� ���� �߰��ؾ��ϴ°͵�

1.  ���̴� �� ���ҽ� �߰��� "FJSystemEngine.cpp" ���� LoadData() �Լ����� ����
    ���̴��� ������۴� Matrix->Vector->Scalar ������ ����ü�� �����ؾ���
    ���̴��� ��������� ������Ҹ� ���̴� ������ �ʱ�ȭ �ʿ�

2.  ������Ʈ �߰��� "CustomComponent.h"�� �ڽ��� �߰��� ������Ʈ�� ����� �ְ� "Object.cpp"�� CObject::CopyObject(const CObject* origin) (line : 251) �Լ� �ȿ� �ڽ��� ������Ʈ��
    else if (ti == typeid(YourComponent))
    {
        cp = obj->AddComponent<YourComponent>();
        memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(YourComponent), (char*)(*iter) + sizeof(Component), COPY_SIZE(YourComponent));
    }
    �������� �߰��ؾ���