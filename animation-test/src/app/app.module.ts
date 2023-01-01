import {NgModule} from '@angular/core';
import {BrowserModule} from '@angular/platform-browser';
import {AppComponent} from './app.component';
import {AnimatedSvgComponent} from './animated-svg/animated-svg.component';


@NgModule({
  declarations: [
    AppComponent,
    AnimatedSvgComponent
  ],
  imports: [
    BrowserModule,
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule {
}
