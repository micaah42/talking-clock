import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FormsModule } from "@angular/forms";
import { HttpClientModule } from '@angular/common/http'

import { MatCardModule } from '@angular/material/card';
import { MatCommonModule } from '@angular/material/core';
import { MatToolbarModule } from "@angular/material/toolbar";
import { MatButtonModule } from "@angular/material/button";
import { MatListModule } from "@angular/material/list";
import { MatFormFieldModule } from "@angular/material/form-field";
import { MatInputModule } from "@angular/material/input";
import { MatCheckboxModule } from "@angular/material/checkbox";
import { MatSlideToggleModule } from "@angular/material/slide-toggle";
import { MatDialogModule } from "@angular/material/dialog";
import { MatRadioModule } from "@angular/material/radio";
import { MatSelectModule } from '@angular/material/select';

import { AlarmsComponent } from './pages/alarms/alarms.component';
import { DevToolsComponent } from './pages/dev-tools/dev-tools.component';
import { OverviewComponent } from './pages/overview/overview.component';
import { AnalogClockComponent } from './components/analog-clock/analog-clock.component';
import { AppComponent } from './app.component';
import { AppRoutingModule } from "./app-routing.module";


@NgModule({
  declarations: [
    AppComponent,
    OverviewComponent,
    AlarmsComponent,
    DevToolsComponent,
    AnalogClockComponent
  ],
  imports: [
    BrowserModule,
    MatCardModule,
    MatCommonModule,
    AppRoutingModule,
    BrowserModule,
    BrowserAnimationsModule,
    MatToolbarModule,
    MatButtonModule,
    MatListModule,
    MatFormFieldModule,
    MatInputModule,
    FormsModule,
    MatCheckboxModule,
    MatSlideToggleModule,
    MatDialogModule,
    MatRadioModule,
    HttpClientModule,
    MatSelectModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule {
}
