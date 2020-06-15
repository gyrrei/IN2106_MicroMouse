/* 
 * File:   dma.h
 * Author: a2-lenz
 *
 * Created on December 10, 2018, 5:01 PM
 */

#ifndef DMA_H
#define DMA_H

#ifdef __cplusplus
#endif
#include <xc.h>
    extern unsigned int adcData[32] __attribute__((space(dma)));
    void initDmaChannel4(void);

#ifdef __cplusplus
}
#endif

#endif /* DMA_H */
