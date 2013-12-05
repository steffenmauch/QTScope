#ifndef __DATAPLOT_INTERFACE_H
#define __DATAPLOT_INTERFACE_H

class DataPlotInterface
{
public:
    DataPlotInterface() {}
    virtual ~DataPlotInterface() {}


private:
    DataPlotInterface( const DataPlotInterface& source );
    void operator = ( const DataPlotInterface& source );
};


#endif // __DATAPLOT_INTERFACE_H
