#ifndef IUPDATE
#define IUPDATE

namespace worker
{
    class IUpdate
    {
        public:
            virtual void update() = 0;
            virtual ~IUpdate() = default;
    };
};

#endif //IUPDATE
