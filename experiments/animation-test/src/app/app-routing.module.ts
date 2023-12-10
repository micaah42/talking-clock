import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { AnimatedSvgComponent as PureMorphComponent } from './animated-svg/animated-svg.component';
import { AnimeJsMorphComponent } from './anime-js-morph/anime-js-morph.component';

export const routes: Routes = [
  { path: 'pure-morph', component: PureMorphComponent },
  { path: 'animejs-morph', component: AnimeJsMorphComponent }
]

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
